#include "module_interface.h"
#include <linux/mutex.h>

#define MAX_BUFFER_SIZE (2040)

extern struct mutex rand_nums_amount_mutex;
extern int *nums_array;
extern int rand_nums_amount;
char r_buffer[BUF_SIZE];

ssize_t node_read(struct file *file, char *buffer, size_t length, loff_t* offset) {
    ssize_t len = length;
    ssize_t copied = 0;
    char tmp_buffer[MAX_BUFFER_SIZE];

    mutex_lock(&rand_nums_amount_mutex);

    if ((nums_array == NULL) || (*offset >= rand_nums_amount * sizeof(int)) ) {
        mutex_unlock(&rand_nums_amount_mutex);
        return 0;
    }

    for (int i = 0; i < rand_nums_amount && copied < sizeof(tmp_buffer); i++) {
        int res = snprintf(tmp_buffer + copied, sizeof(tmp_buffer) - copied,
                           "%d\n", nums_array[i]);
        if (res <= 0) break;
        copied += res;
    }

    if (copied > 0) {
        int err = copy_to_user(buffer, tmp_buffer, copied);
        if(err) {
            mutex_unlock(&rand_nums_amount_mutex);
            return -EFAULT;
        }

        *offset += copied;
    } else {
        printk(KERN_ALERT DEV_NAME " nothing to copy\n");
    }

    // memcpy(tmp_buffer, (char *)nums_array + *offset, len);

    // int err = copy_to_user(buffer, tmp_buffer, len);
    // if(err) {
    //     mutex_unlock(&rand_nums_amount_mutex);
    //     return -EFAULT;
    // }

    // *offset += len;
    mutex_unlock(&rand_nums_amount_mutex);

    return len;
}
