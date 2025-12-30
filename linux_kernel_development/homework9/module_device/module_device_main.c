#include "module.h"
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("User"); 

#define RAND_PERIOD_LOW_BOUNDARY  (1)
#define RAND_PERIOD_HIGH_BOUNDARY (4)

static struct file_operations fops;
static struct class *module_device_class;
static struct cdev module_device_dev;
static dev_t module_device_first;
static int module_device_init_res;

static struct task_struct *thread;

int nums_array[BUF_SIZE];

static int create_device(void) {
    module_device_init_res = alloc_chrdev_region(&module_device_first, 0, 1, DEV_NAME);
    if(module_device_init_res < 0) {
        printk(KERN_ALERT DEV_NAME " allocation failed\n");
        return module_device_init_res;
    }

    module_device_class = class_create("chdevice");
    if(module_device_class == NULL) {
        unregister_chrdev_region(module_device_first, 1);
        printk(KERN_ALERT DEV_NAME " class has not been created\n");
        return -ENODEV;
    }

    if((device_create(module_device_class, NULL, module_device_first, NULL, DEV_NAME)) == NULL) {
        class_destroy(module_device_class);
        unregister_chrdev_region(module_device_first, 1);
        printk(KERN_ALERT DEV_NAME " device has not been created\n");
        return -ENODEV;
    }

    cdev_init(&module_device_dev, &fops);

    if((cdev_add(&module_device_dev, module_device_first, 1)) == -1) {
        device_destroy(module_device_class, module_device_first);
        class_destroy(module_device_class);
        unregister_chrdev_region(module_device_first, 1);
        printk(KERN_ALERT DEV_NAME " files has not been created\n");
        return -ENODEV;
    }

    printk(KERN_ALERT DEV_NAME " registered, major number = %d\n", MAJOR(module_device_first));

    return 0;
}

static int rand_num_get(uint32_t *rand_val_output, int low_limit, int upper_limit) {
    if(rand_val_output == NULL) {
        return -EINVAL;
    }

    if (low_limit >= upper_limit) {
        return -EINVAL;
    }

    uint32_t random_value;
    get_random_bytes(&random_value, sizeof(random_value));

    uint32_t range = upper_limit - low_limit + 1;
    uint32_t bounded_random = low_limit + (random_value % range);

    *rand_val_output = bounded_random;

    return 0;
}

static void generate_random_numbers(int *nums, int nums_amount) {
    for(int i = 0; i < nums_amount; i++) {
        uint32_t random_value;
        get_random_bytes(&random_value, sizeof(random_value));
        printk(KERN_INFO DEV_NAME " generated [%d]", random_value);
        nums[i] = random_value;
    }

    printk("\n");
}

static void sleep_for_rand_seconds(void) {
    uint32_t random_value = 0;
    rand_num_get(&random_value, RAND_PERIOD_LOW_BOUNDARY, RAND_PERIOD_HIGH_BOUNDARY);
    printk(KERN_INFO DEV_NAME " sleeping for %d seconds", random_value);
    msleep((random_value * 1000)); // Multiplying by 1000 to convert to seconds
}

int thread_func(void *arg) {
    char *dev_name = (char *)arg;

    printk(KERN_INFO DEV_NAME " thread dev name: %s", dev_name);

    while(!kthread_should_stop()) {
        sleep_for_rand_seconds();
        lock_writing_mutex(); // Wait if random numbers amount is being changed
        generate_random_numbers(nums_array, rand_nums_amount);
        unlock_writing_mutex();
    }

    return 0;
}

int module_device_init(void) {
    memset(&fops, 0, sizeof(fops));

    fops.owner = THIS_MODULE;
    fops.write = node_write;
    fops.open = node_open;
    fops.release = node_release;

    int err = create_device();
    if(err != 0) {
        printk(KERN_ALERT DEV_NAME " failed to create device");
        return err;
    }

    char thread_name[] = "module_device_thread";

    thread = kthread_create(thread_func, (void*)DEV_NAME, thread_name);
    if(!thread) {
        printk(KERN_ALERT DEV_NAME " kthreaddemo cannot start thread\n");
        kthread_stop(thread);
        return -ENOSYS;
    }

    get_task_struct(thread);
    wake_up_process(thread);
    
    return 0;
}

void module_device_deinit(void) {
    cdev_del(&module_device_dev);
    device_destroy(module_device_class, module_device_first);
    class_destroy(module_device_class);
    unregister_chrdev_region(module_device_first, 1);

    if (thread) {
        kthread_stop(thread);
    }
    put_task_struct(thread);

    printk(KERN_ALERT DEV_NAME " exited\n");
}

module_init(module_device_init);
module_exit(module_device_deinit);
