platform := $(shell uname)

all:
ifeq ($(platform),Darwin)
	gcc -o demo xlib.c spritelib.c -lX11 -L /opt/X11/lib/ -I/opt/X11/include
else
	gcc -o demo xlib.c spritelib.c -lX11
endif

clean:
	rm -f demo

run: all
	./demo

.PHONY: all clean
