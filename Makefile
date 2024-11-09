CC=clang
CFLAGS=-std=gnu18 -I. -g
LDFLAGS=-lm
DEPS=obj/obj.h tgac/tgac.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tgac/tgac_test: tgac/tgac.o tgac/tgac_test.o
	$(CC) -o $@ $^ $(CFLAGS)

tiny_test: tiny_test.o tgac/tgac.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean test

clean:
	rm -f *.o tgac/*.o *.tga *_test
