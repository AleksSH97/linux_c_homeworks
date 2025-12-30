#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

#define DEV_NAME "stop-kthread-dev"
#define STEPS    (100)

MODULE_AUTHOR("user");
MODULE_LICENSE("GPL");

static struct task_struct *thread;

int thread_func(void *arg) {
    long counter = (long)arg;
    int i;

    for (i = 0; i < STEPS; i++) {
        if(counter % 10 == 0) {
            printk(KERN_ALERT "%ld\n", counter);
        }

        msleep(100);
        counter++;

        if(kthread_should_stop())
        {
            break;
        }
    }

    return 0;
}

int stop_kthread_init(void) {
    char thread_name[] = "counter";
    printk(KERN_INFO DEV_NAME " stop-kthread started\n");

    thread = kthread_run(thread_func, (void*)0, thread_name);
    if(!thread) {
        printk(KERN_ALERT DEV_NAME " stop-kthread cannot start thread\n");
        return -ENOSYS;
    }

    printk(KERN_ALERT DEV_NAME " stop-kthread working\n");

    return 0; 
}

void stop_kthread_cleanup(void) {
    if (thread) {
        kthread_stop(thread);
    }
    printk(KERN_ALERT DEV_NAME " stop-kthread exited\n");
}

module_init(stop_kthread_init);
module_exit(stop_kthread_cleanup);
