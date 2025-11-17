#include <linux/module.h>
#include <linux/random.h>

#define DEV_NAME "rand_num"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

static int m_low = 0;
static int m_high = 10;

module_param(m_low, int, 0664);
MODULE_PARM_DESC(m_low, "Low boundary");

module_param(m_high, int, 0664);
MODULE_PARM_DESC(m_high, "High boundary");

static int __init rand_init(void);
static void __exit rand_exit(void);

static int __init rand_init(void) {
    uint32_t random_value;
    get_random_bytes(&random_value, sizeof(random_value));

    random_value = m_low + (random_value % m_high);

    printk(KERN_ALERT "Random number of module from %d to %d: %u\n", m_low, m_high, random_value);
    return 0;
}

static void __exit rand_exit(void) {
    printk(KERN_ALERT DEV_NAME " exited\n");
}

module_init(rand_init);
module_exit(rand_exit);
