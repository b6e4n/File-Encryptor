CC = gcc
CFLAGS = -Wall -O2

MBEDTLS_INC = .
MBEDTLS_LIB = library

all: cypher

cypher: main.o generation.o key.o encrypt.o io.o
	$(CC) $(CFLAGS) -o cypher main.o generation.o key.o encrypt.o io.o -L$(MBEDTLS_LIB) -lmbedtls -lmbedcrypto -lmbedx509

main.o: main.c
	$(CC) $(CFLAGS) -I$(MBEDTLS_INC) -c main.c 

generation.o: generation.c
	$(CC) $(CFLAGS) -I$(MBEDTLS_INC) -c generation.c

key.o: key.c
	$(CC) $(CFLAGS) -I$(MBEDTLS_INC) -c key.c

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -I$(MBEDTLS_INC) -c encrypt.c

io.o: io.c
	$(CC) $(CFLAGS) -I$(MBEDTLS_INC) -c io.c

clean:
	rm -f cypher *.o
