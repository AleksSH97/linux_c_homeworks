#include <linux/module.h>
#include <linux/time.h>

#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

int cur_time(ktime_t *kt) {
    if(kt == NULL) { 
        return -EINVAL;
    }

    *kt = ktime_get_real();

    return 0;
}

EXPORT_SYMBOL(cur_time);
