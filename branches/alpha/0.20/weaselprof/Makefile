CC=/usr/local/arm-apple-darwin/bin/gcc
LD=$(CC)
CFLAGS=-Wall -ObjC
LDFLAGS=-Wl,-syslibroot,$(HEAVENLY) -F$(HEAVENLY)/System/Library/PrivateFrameworks -framework CoreFoundation -framework Foundation -framework vmutils -lobjc

all:	weaselprof

weaselprof:	weaselprof.o

