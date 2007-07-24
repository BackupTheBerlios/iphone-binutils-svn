##
 # Copyright (C) 2007 iZsh 
 # 
 # This software is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public
 # License version 2, as published by the Free Software Foundation.
 #
 # This software is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 #
 # See the GNU General Public License version 2 for more details
 ##
IPHONEROOT = @prefix@

CC = $(IPHONEROOT)/bin/arm-apple-darwin-cc
CCFLAGS = -O0 -dynamiclib
LINKFLAGS = -dynamiclib

RESULT = libarmfp.dylib
SRCS = armfp.c
OBJS = $(SRCS:.c=.o)

all: $(RESULT)

$(RESULT): $(OBJS)
	$(CC) -o $(RESULT) $(CCFLAGS) $(LINKFLAGS) $(OBJS)

clean:
	rm -f $(OBJS) *~

dist-clean: clean
	rm -f $(RESULT)

install: all
	install $(RESULT) $(IPHONEROOT)/lib

%.o: %.c
	$(CC) -c $(LINKFLAGS) $(CCFLAGS) -o $@ $<

