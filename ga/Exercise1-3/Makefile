KDIR:=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/usr/src/kernel
KDIR_LOCAL:=/usr/src/kernels/($shell uname -r)

#PWD:= $(shell pwd)

#CC = i586-poky-linux-gcc
CC = gcc
#ARCH = x86
CROSS_COMPILE = i586-poky-linux-
SROOT=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/

APP = gmem_tester

obj-m:= Mydriver.o


all:
	make ARCH=x86 CROSS_COMPILE=i586-poky-linux- -C $(KDIR) M=$(PWD) modules
	i586-poky-linux-gcc -o $(APP) main.c --sysroot=$(SROOT)
local:
	make -C $(KDIR_LOCAL) M=$(PWD) modules
	gcc -o $(APP) gmem_test.c
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
