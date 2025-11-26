#ifndef _ARRAY_SUM_RAND_H_
#define _ARRAY_SUM_RAND_H_

#include <linux/module.h>

#define DEV_NAME "array_sum_rand"

int rand_num_get(uint32_t *rand_val_output, int low_limit, int upper_limit);

#endif /* _ARRAY_SUM_RAND_H_ */