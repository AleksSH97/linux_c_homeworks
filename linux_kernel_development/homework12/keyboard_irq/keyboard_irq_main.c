#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>

#include "keyboard_irq.h"

MODULE_AUTHOR("user");
MODULE_LICENSE("GPL");

static int irq = 1;
static int dev_id;
int irq_counter = 0;

static struct file_operations fops;
static struct class *keyboard_irq_class;
static struct cdev keyboard_irq_dev;
static dev_t keyboard_irq_first;
static int keyboard_irq_init_res;

int keyboard_irq_init(void);
void keyboard_irq_cleanup(void);

static irqreturn_t kb_int(int irq, void *dev_id) {
    irq_counter++;
    printk(KERN_INFO DEV_NAME " counter = %d\n", irq_counter);
    return IRQ_NONE;
}

static int create_device(void) {
    keyboard_irq_init_res = alloc_chrdev_region(&keyboard_irq_first, 0, 1, DEV_NAME);
    if(keyboard_irq_init_res < 0) {
        printk(KERN_ALERT DEV_NAME " allocation failed\n");
        return keyboard_irq_init_res;
    }

    keyboard_irq_class = class_create("keyboard_irq_chrdev");
    if(keyboard_irq_class == NULL) {
        unregister_chrdev_region(keyboard_irq_first, 1);
        printk(KERN_ALERT DEV_NAME " class has not been created\n");
        return -ENODEV;
    }

    if((device_create(keyboard_irq_class, NULL, keyboard_irq_first, NULL, DEV_NAME)) == NULL) {
        class_destroy(keyboard_irq_class);
        unregister_chrdev_region(keyboard_irq_first, 1);
        printk(KERN_ALERT DEV_NAME " device has not been created\n");
        return -ENODEV;
    }

    cdev_init(&keyboard_irq_dev, &fops);

    if((cdev_add(&keyboard_irq_dev, keyboard_irq_first, 1)) == -1) {
        device_destroy(keyboard_irq_class, keyboard_irq_first);
        class_destroy(keyboard_irq_class);
        unregister_chrdev_region(keyboard_irq_first, 1);
        printk(KERN_ALERT DEV_NAME " files has not been created\n");
        return -ENODEV;
    }

    printk(KERN_ALERT DEV_NAME " registered, major number = %d\n", MAJOR(keyboard_irq_first));

    return 0;
}

int keyboard_irq_init(void) {
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

    if(request_irq(irq, kb_int, IRQF_SHARED, "kb_int", &dev_id)) {
        return -1;
    }

    printk(KERN_ALERT DEV_NAME " started\n");
    return 0;
}

void keyboard_irq_cleanup(void) {
    cdev_del(&keyboard_irq_dev);
    device_destroy(keyboard_irq_class, keyboard_irq_first);
    class_destroy(keyboard_irq_class);
    unregister_chrdev_region(keyboard_irq_first, 1);
    synchronize_irq(irq);
    free_irq(irq, &dev_id);
    printk(KERN_ALERT DEV_NAME " finished\n");
}

module_init(keyboard_irq_init);
module_exit(keyboard_irq_cleanup);
