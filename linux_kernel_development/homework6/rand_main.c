#include "rand.h"
#include <linux/cdev.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("User");

static struct proc_ops rand_proc_ops;
static struct file_operations rand_fops;

static struct class *rand_class;
static struct cdev rand_dev;
static dev_t rand_first;
static int rand_init;

char r_buffer[BUF_SIZE] = "rand message\n";

int init_rand(void) {
    struct proc_dir_entry *proc_node = 0;

    memset(&rand_proc_ops, 0, sizeof(rand_proc_ops));
    memset(&rand_fops, 0, sizeof(rand_fops));
    
    // rand_proc_ops.proc_open = node_open;
    // rand_proc_ops.proc_release = node_release;
    // rand_proc_ops.proc_read = node_read;
    rand_proc_ops.proc_write = node_write;

    rand_fops.owner = THIS_MODULE;
    rand_fops.open = node_open;
    rand_fops.release = node_release;
    rand_fops.read = node_read;

    if(!(proc_node = proc_create(DEV_NAME, 0, NULL, &rand_proc_ops))) {
        printk(KERN_ALERT DEV_NAME " can not create /proc file\n");
        return -EACCES;
    }
    
    rand_init = alloc_chrdev_region(&rand_first, 0, 1, DEV_NAME);
    if(rand_init < 0) {
        printk(KERN_ALERT DEV_NAME " allocation failed\n");
        return rand_init;
    }

    rand_class = class_create("chdevice");
    if(rand_class == NULL) {
        unregister_chrdev_region(rand_first, 1);
        printk(KERN_ALERT DEV_NAME " class has not been created\n");
        return -ENODEV;
    }

    if((device_create(rand_class, NULL, rand_first, NULL, DEV_NAME)) == NULL) {
        class_destroy(rand_class);
        unregister_chrdev_region(rand_first, 1);
        printk(KERN_ALERT DEV_NAME " device has not been created\n");
        return -ENODEV;
    }

    cdev_init(&rand_dev, &rand_fops);

    if((cdev_add(&rand_dev, rand_first, 1)) == -1) {
        device_destroy(rand_class, rand_first);
        class_destroy(rand_class);
        unregister_chrdev_region(rand_first, 1);
        printk(KERN_ALERT DEV_NAME " files has not been created\n");
        return -ENODEV;
    }

    printk(KERN_ALERT DEV_NAME " registered, major number = %d\n", MAJOR(rand_first));
    printk(KERN_ALERT DEV_NAME " started\n");

    return 0;
}

void deinit_rand(void) {
    cdev_del(&rand_dev);
    device_destroy(rand_class, rand_first);
    class_destroy(rand_class);
    unregister_chrdev_region(rand_first, 1);
    remove_proc_entry(DEV_NAME, NULL);
    printk(KERN_ALERT DEV_NAME " exited\n");
}

module_init(init_rand);
module_exit(deinit_rand);
