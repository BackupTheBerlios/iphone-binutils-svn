; It's a bad idea to go recursively traipsing through types without a safety 
; net.

; RUN: llvm-upgrade < %s | llvm-as > %t.out1.bc
; RUN: echo "%S = type { %S*, int* }" | llvm-upgrade | llvm-as > %t.out2.bc
; RUN: llvm-link %t.out1.bc %t.out2.bc

%S = type { %S*, opaque* }
