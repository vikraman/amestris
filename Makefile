CC=gcc
CFLAGS=-O2 -Wall -Wextra -s -pipe

all: virt_kbd virt_mouse start

start:
	chmod +x start.sh

clean:
	rm -f virt_kbd virt_mouse
