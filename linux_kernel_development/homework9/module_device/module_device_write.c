#include "module_device.h"
#include <linux/mutex.h>

DEFINE_MUTEX(rand_nums_amount_mutex);
EXPORT_SYMBOL(rand_nums_amount_mutex);
long rand_nums_amount = 5; // Default random nums amount is 5
char w_buffer[BUF_SIZE];

void lock_writing_mutex(void) {
    mutex_lock(&rand_nums_amount_mutex);
}

void unlock_writing_mutex(void) {
    mutex_unlock(&rand_nums_amount_mutex);
}

ssize_t node_write(struct file *file, const char *buffer, size_t length, loff_t* offset) {
    int ofs = *offset;
    char *p;
    
    if(ofs >= (BUF_SIZE - 1)) {
        return 0;
    }

    if(ofs + length > (BUF_SIZE - 1)) {
        length = BUF_SIZE - 1 - ofs;
    }

    ssize_t len = length;

    for(p = w_buffer + ofs; len > 0; p++, len--, buffer++) {
        get_user(*p, buffer);
    }

    *offset = 0; // Reset offset on every write operation

    *(p - 1) = '\0';

    mutex_lock(&rand_nums_amount_mutex);

    int err = kstrtol(w_buffer, 10, &rand_nums_amount);
    if(err < 0) {
        mutex_unlock(&rand_nums_amount_mutex);
        printk(KERN_ALERT DEV_NAME " failed to convertert string to number: %d", err);
        return err;
    }

    if(rand_nums_amount <= 0 || rand_nums_amount > BUF_SIZE) {
        printk(KERN_ALERT DEV_NAME " wrong amount of random numbers: %ld", rand_nums_amount);
        mutex_unlock(&rand_nums_amount_mutex);
        return -EINVAL;
    }

    mutex_unlock(&rand_nums_amount_mutex);
    printk(KERN_ALERT DEV_NAME " changed amount of random numbers: %ld\n", rand_nums_amount);

    return length;
}
