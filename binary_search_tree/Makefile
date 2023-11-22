GCC = gcc
CFLAGS = -o
CLIBS = -lcunit

SOURCE = source
BUILD = build

main:
	$(GCC) main.c $(CFLAGS) main $(CLIBS)

rerun: clean main
	./main

clean:
	rm -f main
