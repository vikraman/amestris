CC=gcc
CFLAGS=-Wall -Wextra -s -pipe
CFLAGS+=-m32
OBJS=qws.o helpers.o

all: virt

virt: LDFLAGS += -lpthread
virt: virt.c $(OBJS)
	$(CC) $(CFLAGS) -o virt virt.c $(OBJS) $(LDFLAGS)

test: test_execve

clean:
	rm -f virt test_execve *.o
