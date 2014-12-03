KDIR:=/usr/src/kernels/$(shell uname -r)

#PWD:= $(shell pwd)
APP := 0103
CC := gcc
obj-m:= $(APP)km.o

all:
	make -C $(KDIR) M=$(PWD) modules #-f $(KDIR)/Makefile
	#$(CC) -o $(APP) 0103.c
clean:
	@rm -f *.ko
	@rm -f *.o
	@rm -f Module.symvers
	@rm -f modules.order
	@rm -f *.mod.c
	@rm -rf .tmp_versions
	@rm -f *.mod.c
	@rm -f *.mod.o
	@rm -f \.*.cmd
	@rm -f Module.markers
	@rm -f $(APP)

