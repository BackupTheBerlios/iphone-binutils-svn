; RUN: llvm-upgrade < %s | llvm-as | opt -scalarrepl -disable-output
; PR1045

target datalayout = "e-p:32:32"
target endian = little
target pointersize = 32
target triple = "i686-pc-linux-gnu"
	"struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>" = type { "struct.__gnu_cxx::bitmap_allocator<char>::_Alloc_block"* }
	"struct.__gnu_cxx::bitmap_allocator<char>" = type { ubyte }
	"struct.__gnu_cxx::bitmap_allocator<char>::_Alloc_block" = type { [8 x sbyte] }

implementation   ; Functions:

void %_ZN9__gnu_cxx16bitmap_allocatorIwE27_M_deallocate_single_objectEPw() {
entry:
	%this_addr.i = alloca "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"*		; <"struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"**> [#uses=3]
	%tmp = alloca "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>", align 4		; <"struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"*> [#uses=1]
	store "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"* %tmp, "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"** %this_addr.i
	%tmp.i = load "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"** %this_addr.i		; <"struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"*> [#uses=1]
	%tmp.i = bitcast "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"* %tmp.i to "struct.__gnu_cxx::bitmap_allocator<char>"*		; <"struct.__gnu_cxx::bitmap_allocator<char>"*> [#uses=0]
	%tmp1.i = load "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"** %this_addr.i		; <"struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"*> [#uses=1]
	%tmp.i = getelementptr "struct.__gnu_cxx::balloc::_Inclusive_between<__gnu_cxx::bitmap_allocator<char>::_Alloc_block*>"* %tmp1.i, int 0, uint 0		; <"struct.__gnu_cxx::bitmap_allocator<char>::_Alloc_block"**> [#uses=0]
	unreachable
}
