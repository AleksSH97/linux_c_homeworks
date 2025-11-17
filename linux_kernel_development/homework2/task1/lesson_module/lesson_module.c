#include "lesson_module.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

static int __init lesson_module_init(void) {
    ktime_t kt;

    int err = cur_time(&kt);
    if(err) {
        printk(KERN_ALERT "Error (%d): getting current time\n", err);
        return err;
    }

    printk(KERN_ALERT "Got current time: %llu\n", ktime_to_ns(kt));

    return 0;
}

static void __exit lesson_module_exit(void) {
    printk(KERN_ALERT "Lesson module exit\n");
}

module_init(lesson_module_init);
module_exit(lesson_module_exit);