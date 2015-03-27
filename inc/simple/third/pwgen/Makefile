#
#  Makefile
#  
#  Copyright (C) 2013
#  Paul E. Jones <paulej@packetizer.com>
#
#  Description:
#	This is a makefile for Linux to build the program pwgen.
#

CC	= gcc

CFLAGS	= -c -O2 -Wall -D_FILE_OFFSET_BITS=64

LIBS	=

OBJS	= pwgen.o

all: pwgen

pwgen: pwgen.o $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

install: pwgen
	install -o root -g root -m 755 pwgen /usr/bin

uninstall:
	rm -f /usr/bin/pwgen

clean:
	$(RM) *.o pwgen
