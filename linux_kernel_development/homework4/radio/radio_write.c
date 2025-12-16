#include "radio.h"

#define DATA_SIZE (512U) // Accept 512 bytes at most from user

ssize_t radio_write(struct file *file, const char __user *buf, size_t length, loff_t* offset) {
    if(file == NULL) {
        return -EINVAL;
    }

    if(buf == NULL) {
        return -EINVAL;
    }

    if(offset == NULL) { 
        return -EINVAL;
    }

    if(length > DATA_SIZE) {
        return -EINVAL;
    }

    // int ofs = *offset;

    // if(ofs + length) {
    //     length = msg_length - ofs;
    // }

    // ssize_t len = length;
    char p[DATA_SIZE];
    uint16_t length_of_msg;

    int ret = copy_from_user(p, buf, length);
    if(ret != 0) {
        return -EFAULT;
    } else {
        printk(DEV_NAME KERN_ALERT "Received from user: %s\n", p);
        return strnlen(p, length);
    }

    return length_of_msg;
}
