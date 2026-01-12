#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

#define DEV_NAME    "tictac-dev"
#define COUNT_TIMES (10)

MODULE_AUTHOR("user");
MODULE_LICENSE("GPL");

static struct task_struct *tic_thread;
static struct task_struct *tac_thread;

static struct mutex tic_tac_mtx;

static volatile int turn = 1;

int tic_thread_func(void *data);
int tac_thread_func(void *data);
int tictac_init(void);
void tictac_cleanup(void);

int tic_thread_func(void *data) {
    int counter = 0;

    while(counter < COUNT_TIMES) {
        while(turn != 1) {
            schedule();
        }

        mutex_lock(&tic_tac_mtx);
        printk(KERN_INFO DEV_NAME " TIC\n");
        turn = 2;
        mutex_unlock(&tic_tac_mtx);

        counter++;
    }

    return 0;
}

int tac_thread_func(void *data) {
    int counter = 0;

    while(counter < COUNT_TIMES) {
        while(turn != 2) {
            schedule();
        }

        mutex_lock(&tic_tac_mtx);
        printk(KERN_INFO DEV_NAME " TAC\n");
        turn = 1;
        mutex_unlock(&tic_tac_mtx);

        counter++;
    }

    return 0;
}

int tictac_init(void) {
    char tic_thread_name[] = "tic_thread";
    char tac_thread_name[] = "tac_thread";
    printk(KERN_ALERT DEV_NAME " started\n");

    mutex_init(&tic_tac_mtx);

    tic_thread = kthread_create(tic_thread_func, (void *)1, tic_thread_name);
    if(!tic_thread) {
        printk(KERN_ALERT DEV_NAME " cannot start %s\n", tic_thread_name);
        mutex_destroy(&tic_tac_mtx);
        return -ENOSYS;
    }
    get_task_struct(tic_thread);
    wake_up_process(tic_thread);

    tac_thread = kthread_create(tac_thread_func, (void *)1, tac_thread_name);
    if(!tac_thread) {
        printk(KERN_ALERT DEV_NAME " cannot start %s\n", tac_thread_name);
        mutex_destroy(&tic_tac_mtx);
        return -ENOSYS;
    }

    get_task_struct(tac_thread);
    wake_up_process(tac_thread);

    return 0;
}

void tictac_cleanup(void) {
    kthread_stop(tic_thread);
    put_task_struct(tic_thread);
    kthread_stop(tac_thread);
    put_task_struct(tac_thread);
    mutex_destroy(&tic_tac_mtx);
    printk(KERN_ALERT DEV_NAME " exited\n");
}

module_init(tictac_init);
module_exit(tictac_cleanup);
