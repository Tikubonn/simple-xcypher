
export GCC=gcc
export CFLAGS+=-I$(SIMPLE_XCYPHER_INCLUDE) -L$(SIMPLE_XCYPHER_LIB) -g

.PHONY: all
all: bin/test
	gdb bin/test

.PHONY: test-bin
test-bin: scripts/test-simple-xcypher.bash scripts/test.bash
	env PATH="$(PATH):$(SIMPLE_XCYPHER_BIN)" ./scripts/test-simple-xcypher.bash

.PHONY: clean
clean: 
	rm -f src/test-simple-xcypher-calc-encrypted-data-size.o
	rm -f src/test-simple-xcypher-encrypt-decrypt.o
	rm -f src/main.o

src/test-simple-xcypher-calc-encrypted-data-size.o: src/test-simple-xcypher-calc-encrypted-data-size.c src/test-simple-xcypher-calc-encrypted-data-size.h $(SIMPLE_XCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-calc-encrypted-data-size.c 

src/test-simple-xcypher-encrypt-decrypt.o: src/test-simple-xcypher-encrypt-decrypt.c src/test-simple-xcypher-encrypt-decrypt.h src/test.h $(SIMPLE_XCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-xcypher-encrypt-decrypt.o src/test-simple-xcypher-encrypt-decrypt.c

src/main.o: src/main.c src/test-simple-xcypher-calc-encrypted-data-size.h src/test-simple-xcypher-encrypt-decrypt.h $(SIMPLE_XCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

bin: 
	mkdir -p bin

bin/test: src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-encrypt-decrypt.o src/main.o $(SIMPLE_XCYPHER_LIB) | bin
	$(GCC) $(CFLAGS) -o bin/test src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-encrypt-decrypt.o src/main.o -lsimplexcypher
