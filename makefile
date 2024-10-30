
export GCC=gcc
export CFLAGS=-Wall
# export CFLAGS=-Wall -g -Og #for debug build.
# export CFLAGS=-Wall -O3 -finline-functions #for release build.

all: dist/include/simple-xcypher/simple-xcypher.h dist/lib/libsimplexcypher.a dist/lib/libsimplexcypher.so bin/simple-xcypher

.PHONY: clean
clean:
	rm -f src/simple-xcypher.o
	rm -f src/main.o
	make -C test clean

.PHONY: test
test: all 
	make -C test SIMPLE_PCYPHER_INCLUDE=$(CURDIR)/dist/include SIMPLE_PCYPHER_LIB=$(CURDIR)/dist/lib

src/simple-xcypher.o: src/simple-xcypher.c src/simple-xcypher.h
	$(GCC) $(CFLAGS) -c -o src/simple-xcypher.o src/simple-xcypher.c

dist/include/simple-xcypher:
	mkdir -p dist/include/simple-xcypher

dist/include/simple-xcypher/simple-xcypher.h: src/simple-xcypher.h | dist/include/simple-xcypher
	cp src/simple-xcypher.h dist/include/simple-xcypher/simple-xcypher.h

dist/lib:
	mkdir -p dist/lib

dist/lib/libsimplexcypher.a: src/simple-xcypher.o | dist/lib
	ar r dist/lib/libsimplexcypher.a src/simple-xcypher.o

dist/lib/libsimplexcypher.so: src/simple-xcypher.o | dist/lib
	$(GCC) $(CFLAGS) -shared -o dist/lib/libsimplexcypher.so src/simple-xcypher.o

bin:
	mkdir -p bin

src/main.o: src/main.c src/buffer.h dist/include
	$(GCC) $(CFLAGS) -Idist/include -c -o src/main.o src/main.c

bin/simple-xcypher: src/main.o dist/lib/libsimplexcypher.a | bin
	$(GCC) $(CFLAGS) -Ldist/lib -o bin/simple-xcypher src/main.o -lsimplexcypher
