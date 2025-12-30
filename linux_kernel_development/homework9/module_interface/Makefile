KBUILD_EXTRA_SYMBOLS = /home/ashabalin/learning/linux_c_homeworks/linux_kernel_development/homework9/module_device/Module.symvers

CURRENT = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

obj-m := module_interface.o

default: 
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean