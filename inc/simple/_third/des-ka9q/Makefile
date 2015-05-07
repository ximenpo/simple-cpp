# Makefile for portable C version under UNIX
CC=gcc
CFLAGS=-O4
all: certify descert des3cert descycle destime des3time
certify: descert des3cert testdata
	./descert < testdata
	./des3cert < testdata

clean:
	rm -f *.o descert des3cert descycle destime des3time dessp.c gensp

descert: descert.o deskey.o desport.o dessp.o
	$(CC) -o descert descert.o deskey.o desport.o dessp.o

des3cert: des3cert.o deskey.o des3port.o dessp.o
	$(CC) -o des3cert des3cert.o deskey.o des3port.o dessp.o

descycle: descycle.o deskey.o desport.o dessp.o
	$(CC) -o descycle descycle.o deskey.o desport.o dessp.o

destime: unixtime.o deskey.o desport.o dessp.o
	$(CC) -o destime unixtime.o deskey.o desport.o dessp.o

des3time: unixtime3.o deskey.o des3port.o dessp.o
	$(CC) -o des3time unixtime3.o deskey.o des3port.o dessp.o

dessp.c: gensp
	./gensp c > dessp.c

gensp: gensp.c
	$(CC) -O -o gensp gensp.c

deskey.o: deskey.c des.h
desport.o: desport.c des.h
des3port.o: des3port.c des.h
