#
# Makefile for AESStringCrypt test script
# Copyright (C) 2007, 2008, 2009, 2012
# Paul E. Jones <paulej@packetizer.com>
#
# This software is licensed as "freeware."  Permission to distribute
# this software in source and binary forms is hereby granted without a
# fee.  THIS SOFTWARE IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
# THE AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DAMAGES RESULTING FROM THE
# USE OF THIS SOFTWARE, EITHER DIRECTLY OR INDIRECTLY, INCLUDING, BUT
# NOT LIMITED TO, LOSS OF DATA OR DATA BEING RENDERED INACCURATE.
#

CC=gcc

#The below line is for debugging
#CFLAGS=-I. -ggdb -Wall -D_FILE_OFFSET_BITS=64
CFLAGS=-Wall -D_FILE_OFFSET_BITS=64

LIBS=

#Uncomment the line below to compile on Mac
#LIBS=-liconv

sctest: sctest.o AESStringCrypt.o aes.o sha256.o password.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f *.o sctest
