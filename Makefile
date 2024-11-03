CC=clang
CFLAGS=-I. -I../tgac -L../tgac -ltgac -g
DEPS=wavefront.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tiny_test: tiny_test.c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean test

clean:
	rm -f *.o *.a *_test

test: tiny_test
	./tiny_test
