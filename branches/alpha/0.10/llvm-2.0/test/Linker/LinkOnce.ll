; This fails because the linker renames the non-opaque type not the opaque 
; one...

; RUN: echo "%X = linkonce global int 8" | llvm-upgrade | llvm-as > %t.2.bc
; RUN: llvm-upgrade < %s | llvm-as > %t.1.bc
; RUN: llvm-link %t.1.bc %t.2.bc | llvm-dis

%X = linkonce global int 7
