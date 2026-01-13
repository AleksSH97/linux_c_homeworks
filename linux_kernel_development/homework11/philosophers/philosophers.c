#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/time.h>

#define THREAD_NAME_LEN     (20)
#define PHILOSOPHERS_NUMBER (5)
#define DEV_NAME            "philosophers-dev"

MODULE_AUTHOR("user");
MODULE_LICENSE("GPL");

static char *philosophers[] = {
    "Laozi",
    "Plato",
    "Confucius",
    "Aristotle",
    "Socrates",
};

static char thread_names[PHILOSOPHERS_NUMBER][THREAD_NAME_LEN];
static struct mutex sticks[PHILOSOPHERS_NUMBER];
static struct task_struct *threads[PHILOSOPHERS_NUMBER];
static int philosophers_id[PHILOSOPHERS_NUMBER];
static int thread_start_counter = 0;
static long phil_eat_time_ms[PHILOSOPHERS_NUMBER];

int thread_func(void *data);
int philosophers_init(void);
void philosophers_cleanup(void);

static inline int put_left_stick(struct mutex *sticks, int phil_id) {
    if(phil_id >= PHILOSOPHERS_NUMBER) {
        printk(KERN_ALERT DEV_NAME " failed to put left stick: %d\n", phil_id);
        return -EINVAL;
    }

    mutex_unlock(&sticks[phil_id]);

    return 0;
}

static inline int put_right_stick(struct mutex *sticks, int phil_id) {
    if(phil_id >= PHILOSOPHERS_NUMBER) {
        printk(KERN_ALERT DEV_NAME " failed to put right stick: %d\n", phil_id);
        return -EINVAL;
    }

    if(phil_id == (PHILOSOPHERS_NUMBER - 1)) {
        phil_id = 0;
    }

    mutex_unlock(&sticks[phil_id + 1]);

    return 0;
}

static inline int take_left_stick(struct mutex *sticks, int phil_id) {
    if(phil_id >= PHILOSOPHERS_NUMBER) {
        printk(KERN_ALERT DEV_NAME " failed to get left stick: %d\n", phil_id);
        return -EINVAL;
    }

    int res = mutex_trylock(&sticks[phil_id]);
    if(res) {
        printk(KERN_ALERT DEV_NAME " %s failed to take LEFT stick\n", philosophers[phil_id]);
    }
    return res;
}

static inline int take_right_stick(struct mutex *sticks, int phil_id) {
    if(phil_id >= PHILOSOPHERS_NUMBER) {
        printk(KERN_ALERT DEV_NAME " %s failed to get right stick: %d\n", philosophers[phil_id]);
        return -EINVAL;
    }

    if(phil_id == (PHILOSOPHERS_NUMBER - 1)) {
        phil_id = 0;
    }

    int res = mutex_trylock(&sticks[phil_id + 1]);
    if(res) {
        printk(KERN_ALERT DEV_NAME " %s failed to take RIGHT stick\n", philosophers[phil_id]);
    }
    return res;
}

int thread_func(void *data) {
    int *phil_id_ptr = (int *)data;
    int philosopher_id = *phil_id_ptr;

    thread_start_counter++;

    while(thread_start_counter != PHILOSOPHERS_NUMBER) {
        schedule();
    }

    while(!kthread_should_stop()) {
        msleep(100);
        
        printk(KERN_INFO DEV_NAME " %s tries to eat\n", philosophers[philosopher_id]);
        ktime_t start_time = ktime_get_real();

        if(take_right_stick(sticks, philosopher_id)) {
            printk(KERN_ALERT DEV_NAME " %s starts thinking again\n", philosophers[philosopher_id]);
            continue;
        }

        if(take_left_stick(sticks, philosopher_id)) {
            printk(KERN_ALERT DEV_NAME " %s starts thinking again and puts right stick back to the table\n", philosophers[philosopher_id]);
            put_right_stick(sticks, philosopher_id);
            continue;
        }

        printk(KERN_INFO DEV_NAME " %s eats\n", philosophers[philosopher_id], philosopher_id);
        phil_eat_time_ms[philosopher_id] += ((ktime_get_real() - start_time) / 1000); // nanoseconds to ms
        put_left_stick(sticks, philosopher_id);
        put_right_stick(sticks, philosopher_id);
    }

    return 0;
}

int philosophers_init(void) {
    // Fill data according to the amount of philosophers
    for(int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
        int res = snprintf(thread_names[i], THREAD_NAME_LEN, "philosopher_thread_%d\n", i);
        if (res <= 0) return -EINVAL;

        mutex_init(&sticks[i]);
        
        philosophers_id[i] = i;

        threads[i] = kthread_create(thread_func, (int *)&philosophers_id[i], thread_names[i]);
        if(!threads[i]) {
            printk(KERN_ALERT DEV_NAME " cannot start thread %d!\n", i);
            mutex_destroy(&sticks[i]);
            return -ENOSYS;
        }
        get_task_struct(threads[i]);
        wake_up_process(threads[i]);

        printk(KERN_ALERT DEV_NAME " started %d thread\n", i);
    }

    return 0;
}

void philosophers_cleanup(void) {
    for(int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
        kthread_stop(threads[i]);
        put_task_struct(threads[i]);
        mutex_destroy(&sticks[i]);
        printk(KERN_INFO DEV_NAME " %s has been eating for %ld ms\n", philosophers[i], phil_eat_time_ms[i]);
        printk(KERN_ALERT DEV_NAME " stopped %d thread\n", i);
    }
}

module_init(philosophers_init);
module_exit(philosophers_cleanup);
