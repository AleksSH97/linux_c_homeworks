#include "rand.h"
#include <linux/random.h>

DECLARE_WAIT_QUEUE_HEAD(WaitQ);
static int already_opened = 0;

static void generate_random_numbers(int nums_amount) {
    for(int i = 0; i < nums_amount; i++) {
        uint32_t random_value;
        get_random_bytes(&random_value, sizeof(random_value));
        printk(KERN_ALERT DEV_NAME " %d", random_value);
    }
    printk(KERN_ALERT DEV_NAME " \n");
}

int node_open(struct inode *inode, struct file *file) {
    if(already_opened) {
        if(file->f_flags & O_NONBLOCK) {
            return -EAGAIN;
        }
    }
    
    int is_sig = 0;
    int i = 0;

    while(already_opened) {
        wait_event_interruptible(WaitQ, !already_opened);

        for (i = 0; i < _NSIG_WORDS && !is_sig; i++) {
            is_sig = current->pending.signal.sig[i] & ~current->blocked.sig[i];
        }

        if(is_sig) {
            return -EINTR;
        }
    }

    already_opened = 1;

    generate_random_numbers(rand_nums_amount);

    printk(KERN_ALERT DEV_NAME " in use, generated %ld random numbers\n", rand_nums_amount);
    return 0;
}

int node_release(struct inode *inode, struct file *file)
{
    already_opened = 0;
    wake_up(&WaitQ);

    printk(KERN_ALERT DEV_NAME " released\n");
    return 0;
}
