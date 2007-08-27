/* We need to know what prefix to add to function names.  */

#ifndef __USER_LABEL_PREFIX__
#error  __USER_LABEL_PREFIX__ not defined
#endif

/* ANSI concatenation macros.  */

#define CONCAT1(a, b) CONCAT2(a, b)
#define CONCAT2(a, b) a ## b

/* Use the right prefix for global labels.  */

#define SYM(x) CONCAT1 (__USER_LABEL_PREFIX__, x)

#ifdef L_udivsi3
    .text
    .globl SYM(__udivsi3)
SYM(__udivsi3):
            subs              r2,r1,#0x1
            bxeq              lr
            bcc               L_udivsi3_loc00009c
            cmp               r0,r1
            bls               L_udivsi3_loc000080
            tst               r1,r2
            beq               L_udivsi3_loc00008c
            clz               r3,r1
            clz               r2,r0
            sub               r2,r3,r2
            mov               r3,#0x1
            mov               r1,r1,lsl r2
            mov               r3,r3,lsl r2
            mov               r2,#0x0
L_udivsi3_loc000038: cmp               r0,r1
            subcs             r0,r0,r1
            orrcs             r2,r2,r3
            cmp               r0,r1,lsr #1
            subcs             r0,r0,r1,lsr #1
            orrcs             r2,r2,r3,lsr #1
            cmp               r0,r1,lsr #2
            subcs             r0,r0,r1,lsr #2
            orrcs             r2,r2,r3,lsr #2
            cmp               r0,r1,lsr #3
            subcs             r0,r0,r1,lsr #3
            orrcs             r2,r2,r3,lsr #3
            cmp               r0,#0x0
            movnes            r3,r3,lsr #4
            movne             r1,r1,lsr #4
            bne               L_udivsi3_loc000038
            mov               r0,r2
            bx                lr                      @ return 

L_udivsi3_loc000080: moveq             r0,#0x1
            movne             r0,#0x0
            bx                lr                      @ return

L_udivsi3_loc00008c: clz               r2,r1
            rsb               r2,r2,#0x1f
            mov               r0,r0,lsr r2
            bx                lr                      @ return
L_udivsi3_loc00009c: str               lr,[sp,#-0x4]!
            bl                L_udivsi3__aeabi_ldiv0
            mov               r0,#0x0
            ldr               pc,[sp],#0x4            

L_udivsi3__aeabi_ldiv0:
            bx lr
#endif

#ifdef L_umodsi3
    .text
    .globl SYM(__umodsi3)
SYM(__umodsi3):
            subs              r2,r1,#0x1
            bcc               L_umodsi3_loc0000a0
            cmpne             r0,r1
            moveq             r0,#0x0
            tsthi             r1,r2
            andeq             r0,r0,r2
            bxls              lr
            clz               r2,r1
            clz               r3,r0
            sub               r2,r2,r3
            mov               r1,r1,lsl r2
            subs              r2,r2,#0x3
            blt               L_umodsi3_loc000070
L_umodsi3_loc000034: cmp               r0,r1
            subcs             r0,r0,r1
            cmp               r0,r1,lsr #1
            subcs             r0,r0,r1,lsr #1
            cmp               r0,r1,lsr #2
            subcs             r0,r0,r1,lsr #2
            cmp               r0,r1,lsr #3
            subcs             r0,r0,r1,lsr #3
            cmp               r0,#0x1
            mov               r1,r1,lsr #4
            subges            r2,r2,#0x4
            bge               L_umodsi3_loc000034
            tst               r2,#0x3
            teqne             r0,#0x0
            beq               L_umodsi3_loc00009c
L_umodsi3_loc000070: cmn               r2,#0x2
            blt               L_umodsi3_loc000094
            beq               L_umodsi3_loc000088
            cmp               r0,r1
            subcs             r0,r0,r1
            mov               r1,r1,lsr #1
L_umodsi3_loc000088: cmp               r0,r1
            subcs             r0,r0,r1
            mov               r1,r1,lsr #1
L_umodsi3_loc000094: cmp               r0,r1
            subcs             r0,r0,r1
L_umodsi3_loc00009c: bx                lr                   @ return
L_umodsi3_loc0000a0: str               lr,[sp,#-0x4]!
            bl                L_umodsi3__aeabi_ldiv0
            mov               r0,#0x0
            ldr               pc,[sp],#0x4                  @ call -> ?

L_umodsi3__aeabi_ldiv0:
            bx lr
#endif

#ifdef L_modsi3
    .text
    .globl SYM(__modsi3)
SYM(__modsi3):
            cmp               r1,#0x0
            eor               ip,r0,r1
            beq               L_A_0000cc
            rsbmi             r1,r1,#0x0
            subs              r2,r1,#0x1
            beq               L_A_000098
            movs              r3,r0
            rsbmi             r3,r0,#0x0
            cmp               r3,r1
            bls               L_A_0000a4
            tst               r1,r2
            beq               L_A_0000b4
            clz               r2,r1
            clz               r0,r3
            sub               r0,r2,r0
            mov               r2,#0x1
            mov               r1,r1,lsl r0
            mov               r2,r2,lsl r0
            mov               r0,#0x0
L_A_00004c: cmp               r3,r1
            subcs             r3,r3,r1
            orrcs             r0,r0,r2
            cmp               r3,r1,lsr #1
            subcs             r3,r3,r1,lsr #1
            orrcs             r0,r0,r2,lsr #1
            cmp               r3,r1,lsr #2
            subcs             r3,r3,r1,lsr #2
            orrcs             r0,r0,r2,lsr #2
            cmp               r3,r1,lsr #3
            subcs             r3,r3,r1,lsr #3
            orrcs             r0,r0,r2,lsr #3
            cmp               r3,#0x0
            movnes            r2,r2,lsr #4
            movne             r1,r1,lsr #4
            bne               L_A_00004c
            cmp               ip,#0x0
            rsbmi             r0,r0,#0x0
            bx                lr                                @ return
L_A_000098: teq               ip,r0
            rsbmi             r0,r0,#0x0
            bx                lr                                @ return
L_A_0000a4: movcc             r0,#0x0
            moveq             r0,ip,asr #31
            orreq             r0,r0,#0x1
            bx                lr                                @ return
L_A_0000b4: clz               r2,r1
            rsb               r2,r2,#0x1f
            cmp               ip,#0x0
            mov               r0,r3,lsr r2
            rsbmi             r0,r0,#0x0
            bx                lr                                @ return
L_A_0000cc: str               lr,[sp,#-0x4]!
            bl                L_A__aeabi_ldiv0
            mov               r0,#0x0
            ldr               pc,[sp],#0x4                      @ call -> ?

L_A__aeabi_ldiv0:
            bx lr
#endif

#ifdef L_divsi3
    .text
    .globl SYM(__divsi3)
SYM(__divsi3):
            cmp               r1,#0x0
            beq               L_B_0000b8
            rsbmi             r1,r1,#0x0
            movs              ip,r0
            rsbmi             r0,r0,#0x0
            subs              r2,r1,#0x1
            cmpne             r0,r1
            moveq             r0,#0x0
            tsthi             r1,r2
            andeq             r0,r0,r2
            bls               L_B_0000ac
            clz               r2,r1
            clz               r3,r0
            sub               r2,r2,r3
            mov               r1,r1,lsl r2
            subs              r2,r2,#0x3
            blt               L_B_000080
L_B_000044: cmp               r0,r1
            subcs             r0,r0,r1
            cmp               r0,r1,lsr #1
            subcs             r0,r0,r1,lsr #1
            cmp               r0,r1,lsr #2
            subcs             r0,r0,r1,lsr #2
            cmp               r0,r1,lsr #3
            subcs             r0,r0,r1,lsr #3
            cmp               r0,#0x1
            mov               r1,r1,lsr #4
            subges            r2,r2,#0x4
            bge               L_B_000044
            tst               r2,#0x3
            teqne             r0,#0x0
            beq               L_B_0000ac
L_B_000080: cmn               r2,#0x2
            blt               L_B_0000a4
            beq               L_B_000098
            cmp               r0,r1
            subcs             r0,r0,r1
            mov               r1,r1,lsr #1
L_B_000098: cmp               r0,r1
            subcs             r0,r0,r1
            mov               r1,r1,lsr #1
L_B_0000a4: cmp               r0,r1
            subcs             r0,r0,r1
L_B_0000ac: cmp               ip,#0x0
            rsbmi             r0,r0,#0x0
            bx                lr                                @ return
L_B_0000b8: str               lr,[sp,#-0x4]!
            bl                L_B__aeabi_ldiv0
            mov               r0,#0x0
            ldr               pc,[sp],#0x4                      @ call -> ?

L_B__aeabi_ldiv0:
            bx lr
#endif            
            



