#Hello world module example Makefile
ifneq ($(KERNELRELEASE),)
	obj-m :=hello.o
else
	KERNELDIR ?= /home/mike/linux-2.6.20
	#KERNELDIR=/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c 
endif