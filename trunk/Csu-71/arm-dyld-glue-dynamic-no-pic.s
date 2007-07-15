# -----------------------------------------------------------------------------
#   iphone-binutils: development tools for the Apple iPhone        07/13/2007
#   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> but freely
#   redistributable under the terms of the GNU General Public License.
#
#   arm-dyld-glue-pic.s: ARM PIC binding helper routines, part of crt1.o
# -----------------------------------------------------------------------------

    .text
    .private_extern dyld_stub_binding_helper
    .align 2 
dyld_stub_binding_helper:
    str ip,[sp,#-4]!
Ldyld_stub_binding_helper$scv:
    ldr ip,Ldyld_stub_binding_helper$mhsn
    str ip,[sp,#-4]!
Ldyld_stub_binding_helper$scv2:
    ldr ip,Ldyld_stub_binding_helper$dclb
    ldr pc,[ip,#0]

Ldyld_stub_binding_helper$mhsn:
    .long __mh_execute_header
Ldyld_stub_binding_helper$dclb:
    .long Ldyld_content_lazy_binder

    .text
    .private_extern __dyld_func_lookup
    .align 2 
__dyld_func_lookup:
    ldr ip,Ldyld_func_lookup$dcfl
    ldr pc,[ip,#0]

Ldyld_func_lookup$dcfl:
    .long dyld_func_lookup_pointer

    .data
    .align 2 
dyld__mh_execute_header:
    .long __mh_execute_header

    .dyld
    .align 2 
Ldyld_content_lazy_binder:
    .long 0x8fe01000
dyld_func_lookup_pointer:
    .long 0x8fe01008

    .subsections_via_symbols

