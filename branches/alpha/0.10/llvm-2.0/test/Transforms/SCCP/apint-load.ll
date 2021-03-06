; This test makes sure that these instructions are properly constant propagated.

; RUN: llvm-as < %s | opt -ipsccp | llvm-dis | not grep load
; RUN: llvm-as < %s | opt -ipsccp | llvm-dis | not grep fdiv

@X = constant i212 42
@Y = constant [2 x { i212, float }] [ { i212, float } { i212 12, float 1.0 }, 
                                     { i212, float } { i212 37, float 1.2312 } ]
define i212 @test1() {
	%B = load i212* @X
	ret i212 %B
}

define internal float @test2() {
	%A = getelementptr [2 x { i212, float}]* @Y, i32 0, i32 1, i32 1
	%B = load float* %A
	ret float %B
}

define internal i212 @test3() {
	%A = getelementptr [2 x { i212, float}]* @Y, i32 0, i32 0, i32 0
	%B = load i212* %A
	ret i212 %B
}

define float @All()
{
   %A = call float @test2()
   %B = call i212 @test3()
   %C = mul i212 %B, -1234567
   %D = sitofp i212 %C to float
   %E = fdiv float %A, %D
   ret float %E
}


