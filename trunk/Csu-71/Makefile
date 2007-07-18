AS = ./as
CC = ../driver/arm-apple-darwin-gcc
CFLAGS = -g -Wall
LD = ./ld

all:	crt1.o

crt1.o:	start.dynamic_no_pic.o crt.dynamic_no_pic.o dyld_glue.dynamic_no_pic.o dyld
	$(LD) -r -keep_private_externs $^ -o $@

dyld_glue.dynamic_no_pic.o: arm-dyld-glue-dynamic-no-pic.s
	$(AS) -arch armv6 $^ -o $@

start.dynamic_no_pic.o:	arm-start.s
	$(AS) -arch armv6 $^ -o $@

crt.dynamic_no_pic.o:	crt.c
	llvm-gcc -c -emit-llvm -o crt.dynamic_no_pic.bc crt.c
	llc -mtriple=arm-apple-darwin -relocation-model=dynamic-no-pic -mattr=+v6 -f -o crt.dynamic_no_pic.s crt.dynamic_no_pic.bc
	$(AS) -arch armv6 crt.dynamic_no_pic.s -o crt.dynamic_no_pic.o

ppc:	crt1.ppc.o

crt1.ppc.o:	start.dynamic_no_pic.ppc.o crt.dynamic_no_pic.ppc.o dyld_glue.dynamic_no_pic.ppc.o /usr/lib/dyld
	ld -r -keep_private_externs $^ -o $@

dyld_glue.dynamic_no_pic.ppc.o: ppc-dyld-glue-dynamic-no-pic.s
	as -arch ppc $^ -o $@

start.dynamic_no_pic.ppc.o:	ppc-start.s
	as -arch ppc $^ -o $@

crt.dynamic_no_pic.ppc.o:	crt.c
	gcc -arch ppc -c $^ -keep_private_externs -o $@
