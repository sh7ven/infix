extra_flags := -fno-ident -Os
debug := -pedantic -Wall -Werror -Wextra -DDEBUG # TODO: toggle debug
cflags := $(extra_flags) -I $(PWD) -std=c89 -O3 $(debug)
ex = /tmp/cc

.PHONY: clean all

all: test.c
	gcc $(cflags) *.c -o $(ex) $(lib)
	echo "Compiled."
	$(ex)

clean:
	rm $(ex)
