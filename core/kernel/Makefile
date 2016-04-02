obj-m := HazeWorld.o
HazeWorld-objs = hookfun.o haze.o common.o haze_path.o haze_socket.o
KERNELBUILD := /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS += -D__64_BIT_
default:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o *.ko *.mod.c .*.cmd *.markers *.order *.symvers .tmp_versions *.ko.*

install:
	/sbin/insmod ./HazeWorld.ko

remove:
	/sbin/rmmod HazeWorld
