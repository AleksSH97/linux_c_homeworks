#include "keyboard_irq.h"

char r_buffer[BUF_SIZE];

ssize_t node_read(struct file *file, char *buffer, size_t length, loff_t* offset) {
    ssize_t copied = 0;
    char tmp_buffer[1024];

    if (*offset == 0) {
        int res = snprintf(tmp_buffer + copied, sizeof(tmp_buffer) - copied,
                           "%d keyboard IRQs\n", irq_counter);
        if (res <= 0)
            return res;
        copied += res;

        if (copied > 0) {
            if (copy_to_user(buffer, tmp_buffer, copied))
                return -EFAULT;

            *offset += copied;
        } else {
            printk(KERN_ALERT DEV_NAME " nothing to copy\n");
        }
    } else {
        copied = 0;
    }

    return copied;
}
