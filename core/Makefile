obj-m += testhook.o 
KERNELBUILD := /lib/modules/$(shell uname -r)/build

default:
	@echo
	@echo "+-----------------------------------------------------------+"
	@echo "			Klogger Version 1.4                         "
	@echo "			By wzt <xsec.org>                           "
	@echo "			Enjoy Your Hacking                          "
	@echo "+-----------------------------------------------------------+"
	@echo

	make -C $(KERNELBUILD) M=$(shell pwd) modules

clean:
	make -C ${KERNELBUILD} M=$(PWD) $@;
	rm -f *.o *.cmd *.ko *.mod.c *.module.* *.o.cmd
