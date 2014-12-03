APP := 0103

#PWD:= $(shell pwd)
KDIR:=/usr/src/kernels/$(shell uname -r)
CC := gcc
obj-m:= $(APP)km.o

all:
	make -C $(KDIR) M=$(PWD) modules #-f $(KDIR)/Makefile
	#$(CC) -o $(APP) $(APP).c
clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm -f $(APP) $(APP).o

