KBUILD_EXTRA_SYMBOLS = /home/ashabalin/learning/linux_c_homeworks/linux_kernel_development/homework2/task1/cur_time/Module.symvers

CURRENT = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

obj-m := lesson_module.o

default: 
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean