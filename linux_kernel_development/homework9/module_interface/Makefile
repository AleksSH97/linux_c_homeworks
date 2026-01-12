KBUILD_EXTRA_SYMBOLS = /home/ashabalin/learning/homeworks/linux_kernel_development/homework9/module_device/Module.symvers

CURRENT = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

obj-m := module_interface.o

module_interface-objs := module_interface_main.o module_interface_read.o module_interface_or.o

default: 
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean