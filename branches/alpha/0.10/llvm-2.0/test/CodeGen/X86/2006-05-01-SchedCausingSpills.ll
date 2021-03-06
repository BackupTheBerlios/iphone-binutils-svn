; RUN: llvm-upgrade < %s | llvm-as | llc -march=x86 -mcpu=yonah -stats |& \
; RUN:   not grep {Number of register spills}

int %foo(<4 x float>* %a, <4 x float>* %b, <4 x float>* %c, <4 x float>* %d) {
	%tmp44 = load <4 x float>* %a		; <<4 x float>> [#uses=9]
	%tmp46 = load <4 x float>* %b		; <<4 x float>> [#uses=1]
	%tmp48 = load <4 x float>* %c		; <<4 x float>> [#uses=1]
	%tmp50 = load <4 x float>* %d		; <<4 x float>> [#uses=1]
	%tmp51 = cast <4 x float> %tmp44 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp = shufflevector <4 x int> %tmp51, <4 x int> undef, <4 x uint> < uint 3, uint 3, uint 3, uint 3 >		; <<4 x int>> [#uses=2]
	%tmp52 = cast <4 x int> %tmp to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp60 = xor <4 x int> %tmp, < int -2147483648, int -2147483648, int -2147483648, int -2147483648 >		; <<4 x int>> [#uses=1]
	%tmp61 = cast <4 x int> %tmp60 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp74 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp52, <4 x float> %tmp44, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp75 = cast <4 x float> %tmp74 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp88 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp44, <4 x float> %tmp61, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp89 = cast <4 x float> %tmp88 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp98 = tail call <4 x int> %llvm.x86.sse2.packssdw.128( <4 x int> %tmp75, <4 x int> %tmp89 )		; <<4 x int>> [#uses=1]
	%tmp102 = cast <4 x int> %tmp98 to <8 x short>		; <<8 x short>> [#uses=1]
	%tmp = shufflevector <8 x short> %tmp102, <8 x short> undef, <8 x uint> < uint 0, uint 1, uint 2, uint 3, uint 6, uint 5, uint 4, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp105 = shufflevector <8 x short> %tmp, <8 x short> undef, <8 x uint> < uint 2, uint 1, uint 0, uint 3, uint 4, uint 5, uint 6, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp105 = cast <8 x short> %tmp105 to <4 x float>		; <<4 x float>> [#uses=1]
	store <4 x float> %tmp105, <4 x float>* %a
	%tmp108 = cast <4 x float> %tmp46 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp109 = shufflevector <4 x int> %tmp108, <4 x int> undef, <4 x uint> < uint 3, uint 3, uint 3, uint 3 >		; <<4 x int>> [#uses=2]
	%tmp109 = cast <4 x int> %tmp109 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp119 = xor <4 x int> %tmp109, < int -2147483648, int -2147483648, int -2147483648, int -2147483648 >		; <<4 x int>> [#uses=1]
	%tmp120 = cast <4 x int> %tmp119 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp133 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp109, <4 x float> %tmp44, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp134 = cast <4 x float> %tmp133 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp147 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp44, <4 x float> %tmp120, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp148 = cast <4 x float> %tmp147 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp159 = tail call <4 x int> %llvm.x86.sse2.packssdw.128( <4 x int> %tmp134, <4 x int> %tmp148 )		; <<4 x int>> [#uses=1]
	%tmp163 = cast <4 x int> %tmp159 to <8 x short>		; <<8 x short>> [#uses=1]
	%tmp164 = shufflevector <8 x short> %tmp163, <8 x short> undef, <8 x uint> < uint 0, uint 1, uint 2, uint 3, uint 6, uint 5, uint 4, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp166 = shufflevector <8 x short> %tmp164, <8 x short> undef, <8 x uint> < uint 2, uint 1, uint 0, uint 3, uint 4, uint 5, uint 6, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp166 = cast <8 x short> %tmp166 to <4 x float>		; <<4 x float>> [#uses=1]
	store <4 x float> %tmp166, <4 x float>* %b
	%tmp169 = cast <4 x float> %tmp48 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp170 = shufflevector <4 x int> %tmp169, <4 x int> undef, <4 x uint> < uint 3, uint 3, uint 3, uint 3 >		; <<4 x int>> [#uses=2]
	%tmp170 = cast <4 x int> %tmp170 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp180 = xor <4 x int> %tmp170, < int -2147483648, int -2147483648, int -2147483648, int -2147483648 >		; <<4 x int>> [#uses=1]
	%tmp181 = cast <4 x int> %tmp180 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp194 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp170, <4 x float> %tmp44, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp195 = cast <4 x float> %tmp194 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp208 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp44, <4 x float> %tmp181, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp209 = cast <4 x float> %tmp208 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp220 = tail call <4 x int> %llvm.x86.sse2.packssdw.128( <4 x int> %tmp195, <4 x int> %tmp209 )		; <<4 x int>> [#uses=1]
	%tmp224 = cast <4 x int> %tmp220 to <8 x short>		; <<8 x short>> [#uses=1]
	%tmp225 = shufflevector <8 x short> %tmp224, <8 x short> undef, <8 x uint> < uint 0, uint 1, uint 2, uint 3, uint 6, uint 5, uint 4, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp227 = shufflevector <8 x short> %tmp225, <8 x short> undef, <8 x uint> < uint 2, uint 1, uint 0, uint 3, uint 4, uint 5, uint 6, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp227 = cast <8 x short> %tmp227 to <4 x float>		; <<4 x float>> [#uses=1]
	store <4 x float> %tmp227, <4 x float>* %c
	%tmp230 = cast <4 x float> %tmp50 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp231 = shufflevector <4 x int> %tmp230, <4 x int> undef, <4 x uint> < uint 3, uint 3, uint 3, uint 3 >		; <<4 x int>> [#uses=2]
	%tmp231 = cast <4 x int> %tmp231 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp241 = xor <4 x int> %tmp231, < int -2147483648, int -2147483648, int -2147483648, int -2147483648 >		; <<4 x int>> [#uses=1]
	%tmp242 = cast <4 x int> %tmp241 to <4 x float>		; <<4 x float>> [#uses=1]
	%tmp255 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp231, <4 x float> %tmp44, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp256 = cast <4 x float> %tmp255 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp269 = tail call <4 x float> %llvm.x86.sse.cmp.ps( <4 x float> %tmp44, <4 x float> %tmp242, sbyte 1 )		; <<4 x float>> [#uses=1]
	%tmp270 = cast <4 x float> %tmp269 to <4 x int>		; <<4 x int>> [#uses=1]
	%tmp281 = tail call <4 x int> %llvm.x86.sse2.packssdw.128( <4 x int> %tmp256, <4 x int> %tmp270 )		; <<4 x int>> [#uses=1]
	%tmp285 = cast <4 x int> %tmp281 to <8 x short>		; <<8 x short>> [#uses=1]
	%tmp286 = shufflevector <8 x short> %tmp285, <8 x short> undef, <8 x uint> < uint 0, uint 1, uint 2, uint 3, uint 6, uint 5, uint 4, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp288 = shufflevector <8 x short> %tmp286, <8 x short> undef, <8 x uint> < uint 2, uint 1, uint 0, uint 3, uint 4, uint 5, uint 6, uint 7 >		; <<8 x short>> [#uses=1]
	%tmp288 = cast <8 x short> %tmp288 to <4 x float>		; <<4 x float>> [#uses=1]
	store <4 x float> %tmp288, <4 x float>* %d
	ret int 0
}

declare <4 x float> %llvm.x86.sse.cmp.ps(<4 x float>, <4 x float>, sbyte)

declare <4 x int> %llvm.x86.sse2.packssdw.128(<4 x int>, <4 x int>)
