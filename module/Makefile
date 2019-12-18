
KERDIR:= /lib/modules/$(shell uname -r)/build/
#KERDIR:= /home/subcar/linux/linux-4.16
PWD:= $(shell pwd)

obj-m:=demo.o

all:
	make -C $(KERDIR) M=$(PWD) modules

clean:
	make -C $(KERDIR) M=$(PWD) clean

