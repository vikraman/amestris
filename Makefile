CC=gcc
CFLAGS=-Wall -Wextra -s -pipe

all: virt

virt: CFLAGS += -lpthread
virt: virt.c qws.c helpers.c

test: test_execve

clean:
	rm -f virt test_execve *.o
