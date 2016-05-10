ifneq ($(KERNELRELEASE),)
	obj-m:=fc1553.o
else
	KERNELDIR ?= /home/wzl/linux-3.0.48
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.tar.gz *.order *.symvers 
endif
