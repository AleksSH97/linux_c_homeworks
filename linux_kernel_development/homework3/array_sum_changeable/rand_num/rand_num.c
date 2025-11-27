#include <linux/module.h>
#include <linux/random.h>
#include <linux/errno.h>

#define DEV_NAME "rand_num"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

int rand_num_get(uint32_t *rand_val_output, int low_limit, int upper_limit) {
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

EXPORT_SYMBOL(rand_num_get);
