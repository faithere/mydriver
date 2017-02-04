ifneq ($(KERNELRELEASE),)
        obj-m :=button.o
else
        KERNELDIR ?= /home/mike/linux-2.6.20
        PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c 
endif
