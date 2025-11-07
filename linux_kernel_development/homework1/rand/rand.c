#include <linux/module.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

static int __init rand_init(void) {
    uint32_t random_value;
    get_random_bytes(&random_value, sizeof(random_value));
    printk(KERN_ALERT "Random number of module: %u Module start\n", random_value);
    return 0;
}

static void __exit rand_exit(void) { 
    printk(KERN_ALERT "Module exit\n");
    return;
}

module_init(rand_init);
module_exit(rand_exit);
