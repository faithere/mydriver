ifneq ($(KERNELRELEASE),)
        obj-m :=button.o
else
        KERNELDIR ?= /home/wzl/linux-2.6.25
        PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.markers *.symvers *.order
endif
