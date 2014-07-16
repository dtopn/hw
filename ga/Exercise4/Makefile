KDIR:=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/usr/src/kernel
KDIR_LOCAL:=/usr/src/kernel

PWD:= $(shell pwd)

#CC = i586-poky-linux-gcc
CC = gcc
ARCH = x86
CROSS_COMPILE = i586-poky-linux-
SROOT=/opt/clanton-full/1.4.2/sysroots/i586-poky-linux/

APP = 

hpet:
	$(CC) hpet_example.c -o hpet_example


clean:
	rm -f *.o
	rm -f hpet_example
