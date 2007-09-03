@ old-style macro
.macro foo
    add r0,r0,$0
.endm

@ new-style macro with 1 arg
    .macro bar a
    add r0,r0,\a
.endm

@ new-style macro with 5 args
.macro baz a b c d, e
    add \a,\c,\d
.endm

@ new-style macro with 1 arg and default
.macro boo a=r0
    add \a,r1,\a
.endm

@ new-style macro with 5 args and some defaults
.macro wibble a=r0,b,c=r3 d=r5,e=r9
    add \a,\d,\c
.endmacro

foo r1
bar r2
baz e=r1,a=r3,b=r4,d=r5,c=r6
 

