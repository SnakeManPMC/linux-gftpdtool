#
# Makefile --- dophp (c) PMC 2000 - 2015
#

CC	= gcc

INCLUDE = /upload1/code/include

CFLAGS  = -I$(INCLUDE)

all:		dophp

clean:          rm dophp

dophp:          dophp.c
		$(CC) $(CFLAGS) -o $@ $<
