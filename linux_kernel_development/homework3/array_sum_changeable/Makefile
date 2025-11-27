KBUILD_EXTRA_SYMBOLS = /home/ashabalin/learning/homeworks/linux_kernel_development/homework3/array_sum_rand/rand_num/Module.symvers

CURRENT = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

obj-m := array_sum_rand.o

default: 
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean