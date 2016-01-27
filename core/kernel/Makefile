obj-m := HazeWorld.o
HazeWorld-objs = hookfun.o haze.o netlink.o common.o haze_path.o
KERNELBUILD := /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS += -D__HAZE_NL__
default:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o *.ko *.mod.c .*.cmd *.markers *.order *.symvers .tmp_versions *.ko.*

install:
	/sbin/insmod ./HazeWorld.ko

remove:
	/sbin/rmmod HazeWorld
