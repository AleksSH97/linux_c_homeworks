#include <linux/module.h>
#include <linux/slab.h>
#include <linux/errno.h>

#define DEV_NAME "dynamic_dev"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("user");

#define ALLOC_SIZE (256)

int init_dynamic(void)
{
    char* buffer;

    printk(KERN_ALERT DEV_NAME " started\n");
    
    buffer = kmalloc(ALLOC_SIZE, GFP_KERNEL);
    if(buffer == NULL) {
        printk(KERN_ALERT DEV_NAME " failed to allocate %d bytes in kernel", ALLOC_SIZE);
        return -ENOMEM;
    }

    printk(KERN_INFO DEV_NAME " allocated %d bytes", ALLOC_SIZE);
    printk(KERN_INFO " buffer address: %p", buffer);

    kfree(buffer);

    printk(KERN_INFO DEV_NAME " freed %d bytes", ALLOC_SIZE);

    return 0;
}

void cleanup_dynamic(void)
{
    printk(KERN_ALERT DEV_NAME " exited\n");
    return;
}

module_init(init_dynamic);
module_exit(cleanup_dynamic);
