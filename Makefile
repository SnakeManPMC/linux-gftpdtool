#
# Makefile --- gftpdtool (c) PMC 1999 - 2015
#

CC	= gcc

INCLUDE = /code/include

CFLAGS  = -I$(INCLUDE)

all:		gftpdtool log web-who getcomp

clean:          rm gftpdtool log web-who getcomp

gftpdtool:      gftpdtool.c
		$(CC) $(CFLAGS) -o $@ $<

log:		log.c
		$(CC) $(CFLAGS) -o $@ $<

web-who:        web-who.c
		$(CC) $(CFLAGS) -o $@ $<

getcomp:	getcomp.c
		$(CC) $(CFLAGS) -o $@ $<
		
