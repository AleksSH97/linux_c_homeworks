#include <linux/errno.h>
#include <linux/moduleparam.h>

#include "array_sum_rand.h"

/**
    Instruction how to change m_low and m_high:

    In terminal when modules are loaded:
        echo * | sudo tee /sys/module/array_sum_rand/parameters/m_high, where * is your number
        echo * | sudo tee /sys/module/array_sum_rand/parameters/m_low, where * is your number
*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Linux Kernel Module which takes an array of integers and "
                   "low/high limits for random number generation, "
                   "multiplies each array element by a random number, "
                   "and sums the results");

static void calculate_array_sum(void);
static int param_set_low_limit(const char *val, const struct kernel_param *kp);
static int param_set_high_limit(const char *val, const struct kernel_param *kp);

static int array_count;
static int m_array[30];
static int m_low = 0;
static int m_high = 100;

static const struct kernel_param_ops low_param_ops = {
    .set = param_set_low_limit,
    .get = param_get_int,
};

static const struct kernel_param_ops high_param_ops = {
    .set = param_set_high_limit,
    .get = param_get_int,
};

module_param_cb(m_low, &low_param_ops, &m_low, 0644);
MODULE_PARM_DESC(m_low, "Low limit of randomly generated unsigned integer (0 <= low < high)");

module_param_cb(m_high, &high_param_ops, &m_high, 0644);
MODULE_PARM_DESC(m_high, "High limit of randomly generated unsigned integer (high > low)");

module_param_array(m_array, int, &array_count, 0444);
MODULE_PARM_DESC(m_array, "Array of integers to be multiplied by random numbers and summed");

static int param_set_low_limit(const char *val, const struct kernel_param *kp) {
    int num, ret;

    ret = kstrtoint(val, 10, &num);
    if (ret != 0 || num < 0) {
        printk(KERN_ALERT DEV_NAME " fn returned %d or changing low limit to a forbidden value: %d\n", ret, num);
        return -EINVAL;
    }
    
    ret = param_set_int(val, kp);
    if(ret == 0) {
        calculate_array_sum();
    }

    return 0;
}

static int param_set_high_limit(const char *val, const struct kernel_param *kp) {
    int num, ret;

    ret = kstrtoint(val, 10, &num);
    if (ret != 0 || num < 0) {
        printk(KERN_ALERT DEV_NAME " fn returned %d or changing high limit to a forbidden value: %d\n", ret, num);
        return -EINVAL;
    }

    ret = param_set_int(val, kp);
    if(ret == 0) {
        calculate_array_sum();
    }

    return 0;
}

static void calculate_array_sum(void) {
    if (array_count == 0) {
        printk(KERN_ALERT DEV_NAME " array is empty, nothing to recalculate\n");
        return;
    }

    printk(KERN_INFO DEV_NAME " recalculating with new range %d-%d:\n", m_low, m_high);

    int sum = 0;
    for (int i = 0; i < array_count; i++) {
        uint32_t rand_num = 0;
        rand_num_get(&rand_num, m_low, m_high);
        printk(KERN_INFO DEV_NAME " got random number: %d\n", rand_num);
        
        m_array[i] *= rand_num;
        printk(KERN_INFO DEV_NAME " %d ", m_array[i]);
        sum += m_array[i];
    }

    printk(KERN_INFO DEV_NAME " new total sum: %d\n", sum);
}

static int __init array_sum_rand_init(void) {
    if(array_count == 0) {
        printk(KERN_ALERT DEV_NAME " initial array is empty\n");
        return -EINVAL;
    }

    printk(KERN_INFO DEV_NAME " initial array:\n");
    for(uint16_t i = 0; i < array_count; i++) {
        printk(KERN_INFO DEV_NAME " %d ", m_array[i]);
    }

    printk(KERN_INFO DEV_NAME " multiplying all array nums by randomly generated number\n");

    calculate_array_sum();

    return 0;
}

static void __exit array_sum_rand_exit(void) {
    printk(KERN_ALERT DEV_NAME " exit\n");
}

module_init(array_sum_rand_init);
module_exit(array_sum_rand_exit);
