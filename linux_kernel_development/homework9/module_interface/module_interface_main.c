#include "module_interface.h"
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("User");

#define MODULE_INTERFACE_CHDEV_NAME "module_interface_chdev"

static struct file_operations fops;
static struct class *module_interface_class;
static struct cdev module_interface_dev;
static dev_t module_interface_first;
static int module_interface_init_res;

int module_interface_init(void);
void module_interface_deinit(void);

static int create_device(void) {
    module_interface_init_res = alloc_chrdev_region(&module_interface_first, 0, 1, DEV_NAME);
    if(module_interface_init_res < 0) {
        printk(KERN_ALERT DEV_NAME " allocation failed\n");
        return module_interface_init_res;
    }

    module_interface_class = class_create(MODULE_INTERFACE_CHDEV_NAME);
    if(module_interface_class == NULL) {
        unregister_chrdev_region(module_interface_first, 1);
        printk(KERN_ALERT DEV_NAME " class has not been created\n");
        return -ENODEV;
    }

    if((device_create(module_interface_class, NULL, module_interface_first, NULL, DEV_NAME)) == NULL) {
        class_destroy(module_interface_class);
        unregister_chrdev_region(module_interface_first, 1);
        printk(KERN_ALERT DEV_NAME " device has not been created\n");
        return -ENODEV;
    }

    cdev_init(&module_interface_dev, &fops);

    if((cdev_add(&module_interface_dev, module_interface_first, 1)) == -1) {
        device_destroy(module_interface_class, module_interface_first);
        class_destroy(module_interface_class);
        unregister_chrdev_region(module_interface_first, 1);
        printk(KERN_ALERT DEV_NAME " files has not been created\n");
        return -ENODEV;
    }

    printk(KERN_ALERT DEV_NAME " registered, major number = %d\n", MAJOR(module_interface_first));

    return 0;
}

int module_interface_init(void) {
    memset(&fops, 0, sizeof(fops));

    fops.owner = THIS_MODULE;
    fops.read = node_read;
    fops.open = node_open;
    fops.release = node_release;

    int err = create_device();
    if(err != 0) {
        printk(KERN_ALERT DEV_NAME " failed to create device");
        return err;
    }

    // char thread_name[] = "module_interface_thread";

    // thread = kthread_create(thread_func, (void*)DEV_NAME, thread_name);
    // if(!thread) {
    //     printk(KERN_ALERT DEV_NAME " kthreaddemo cannot start thread\n");
    //     kthread_stop(thread);
    //     return -ENOSYS;
    // }

    // get_task_struct(thread);
    // wake_up_process(thread);

    printk(KERN_ALERT DEV_NAME " started\n");
    
    return 0;
}

void module_interface_deinit(void) {
    cdev_del(&module_interface_dev);
    device_destroy(module_interface_class, module_interface_first);
    class_destroy(module_interface_class);
    unregister_chrdev_region(module_interface_first, 1);

    // if (thread) {
        // kthread_stop(thread);
    // }
    // put_task_struct(thread);

    printk(KERN_ALERT DEV_NAME " exited\n");
}

module_init(module_interface_init);
module_exit(module_interface_deinit);
