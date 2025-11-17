#ifndef _TIME_H_
#define _TIME_H_

#include <linux/module.h>

#define DEV_NAME "lesson_module"

int cur_time(ktime_t *kt);

#endif /* _TIME_H_ */