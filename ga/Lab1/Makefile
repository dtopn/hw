KDIR:=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/usr/src/kernel
#KDIR_LOCAL:=/usr/src/kernels/2.6.32-431.20.3.el6.x86_64
KDIR_LOCAL:=/usr/src/kernel

PWD:= $(shell pwd)

#CC = i586-poky-linux-gcc
CC = gcc
CXX = g++
#ARCH = x86
CROSS_COMPILE = i586-poky-linux-
SROOT=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/

APP = Squeue_tester

obj-m:= Squeue.o


all:
	make ARCH=x86 CROSS_COMPILE=i586-poky-linux- -C $(KDIR) M=$(PWD) modules
	i586-poky-linux-gcc -o $(APP) main_lab1.c --sysroot=$(SROOT) -lpthread
local: app
	make -C $(KDIR_LOCAL) M=$(PWD) modules
app:
	$(CC) -o $(APP) main_lab1.c -lpthread
clean:
	rm -f *.ko
	rm -f *.o
	rm -f Module.symvers
	rm -f modules.order
	rm -f *.mod.c
	rm -rf .tmp_versions
	rm -f *.mod.c
	rm -f *.mod.o
	rm -f \.*.cmd
	rm -f Module.markers
	rm -f $(APP) 
