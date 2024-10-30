
export GCC=gcc
export CFLAGS+=-I$(SIMPLE_PCYPHER_INCLUDE) -L$(SIMPLE_PCYPHER_LIB) -g

.PHONY: all
all: bin/test
	gdb bin/test

.PHONY: clean
clean: 
	rm -f src/test-simple-xcypher-calc-encrypted-data-size.o
	rm -f src/test-simple-xcypher-encrypt-decrypt.o
	rm -f src/main.o

src/test-simple-xcypher-calc-encrypted-data-size.o: src/test-simple-xcypher-calc-encrypted-data-size.c src/test-simple-xcypher-calc-encrypted-data-size.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-calc-encrypted-data-size.c 

src/test-simple-xcypher-encrypt-decrypt.o: src/test-simple-xcypher-encrypt-decrypt.c src/test-simple-xcypher-encrypt-decrypt.h src/test.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-xcypher-encrypt-decrypt.o src/test-simple-xcypher-encrypt-decrypt.c

src/main.o: src/main.c src/test-simple-xcypher-calc-encrypted-data-size.h src/test-simple-xcypher-encrypt-decrypt.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

bin: 
	mkdir -p bin

bin/test: src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-encrypt-decrypt.o src/main.o $(SIMPLE_PCYPHER_LIB) | bin
	$(GCC) $(CFLAGS) -o bin/test src/test-simple-xcypher-calc-encrypted-data-size.o src/test-simple-xcypher-encrypt-decrypt.o src/main.o -lsimplexcypher
