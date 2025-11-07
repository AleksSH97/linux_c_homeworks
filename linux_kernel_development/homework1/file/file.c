#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");

static int __init some_file_init(void) {
    struct file *filp;
    char text[] = "Hello from Kernel!\n";

    loff_t pos = 0;
    ssize_t bytes_written;

    filp = filp_open("/tmp/my_kernel_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    bytes_written = kernel_write(filp, text, strlen(text), &pos);

    filp_close(filp, NULL);

    printk(KERN_ALERT "Module start\n");
    return 0;
}

static void __exit some_file_exit(void) { 
    printk(KERN_ALERT "Module exit\n");
    return;
}

module_init(some_file_init);
module_exit(some_file_exit);
