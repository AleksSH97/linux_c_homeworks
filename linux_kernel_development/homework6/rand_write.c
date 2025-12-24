#include "rand.h"

long rand_nums_amount = 0;

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

    for(p = r_buffer + ofs; len > 0; p++, len--, buffer++) {
        get_user(*p, buffer);
    }

    *offset += length;

    *(p - 1) = '\0';

    int err = kstrtol(r_buffer, 10, &rand_nums_amount);
    if(err < 0) {
        printk(KERN_ALERT DEV_NAME " failed to convertert string to number: %d", err);
        return err;
    }

    printk(KERN_ALERT DEV_NAME " changed amount of random numbers: %ld\n", rand_nums_amount);

    return length;
}
