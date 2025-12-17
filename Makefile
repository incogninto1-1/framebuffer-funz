all:
	gcc -o demo xlib.c spritelib.c -lX11

clean:
	rm -f demo

run: all
	./demo

.PHONY: all clean