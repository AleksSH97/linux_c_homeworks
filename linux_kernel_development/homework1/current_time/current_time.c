#include <linux/module.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

static int __init cur_time_init(void) {
    ktime_t kt;

    kt = ktime_get_real();

    printk(KERN_ALERT "Module start at %llu\n", ktime_to_ns(kt));
    return 0;
}

static void __exit cur_time_exit(void) { 
    printk(KERN_ALERT "Module exit\n");
    return;
}

module_init(cur_time_init);
module_exit(cur_time_exit);
