#include <linux/module.h>
#include <linux/errno.h>

#include "array_sum_rand.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Linux Kernel Module which takes an array of integers and "
                   "low/high limits for random number generation, "
                   "multiplies each array element by a random number, "
                   "and sums the results");

static int array_count;
static int m_array[30];
static int m_low = 0;
static int m_high = 100;

module_param(m_low, int, 0664);
MODULE_PARM_DESC(m_low, "Low limit of randomly generated unsigned integer");

module_param(m_high, int, 0664);
MODULE_PARM_DESC(m_high, "High limit of randomly generated unsigned integer");

module_param_array(m_array, int, &array_count, 0660); /* 0660 where last 0 means that parameter cannot be changed after module is loaded */
MODULE_PARM_DESC(m_array, "Array of integers which are going to be multiplied by randomly generated unsigned integer and then summarized into one number");

static int __init array_sum_rand_init(void) {
    if(array_count == 0) {
        printk(KERN_ALERT DEV_NAME " initial array is emptry\n");
        return -EINVAL;
    }

    printk(KERN_INFO DEV_NAME " initial array:\n");
    for(uint16_t i = 0; i < array_count; i++) {
        printk(KERN_INFO DEV_NAME " %d ", m_array[i]);
    }

    printk(KERN_INFO DEV_NAME " multiplying all array nums by randomly generated number\n");
    int sum = 0;
    for(uint16_t i = 0; i < array_count; i++) {
        uint32_t rand_num = 0;
        rand_num_get(&rand_num, m_low, m_high);
        printk(KERN_INFO DEV_NAME " got random number: %d\n", rand_num);

        m_array[i] *= rand_num;
        printk(KERN_INFO DEV_NAME " %d ", m_array[i]);
        sum += m_array[i];
    }

    printk(KERN_INFO DEV_NAME " sum of all numbers in the array after multiplying: %d\n", sum);

    return 0;
}

static void __exit array_sum_rand_exit(void) {
    printk(KERN_ALERT DEV_NAME " exit\n");
}

module_init(array_sum_rand_init);
module_exit(array_sum_rand_exit);
