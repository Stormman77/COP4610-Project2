obj-m += elevator.o

ccflags-y := -g -Wno-error

KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)/src modules
	gcc -o producer tests/elevator-test/producer.c
	gcc -o consumer tests/elevator-test/consumer.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD)/src clean
	rm producer
	rm consumer