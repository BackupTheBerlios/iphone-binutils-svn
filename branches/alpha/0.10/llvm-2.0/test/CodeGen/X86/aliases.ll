; RUN: llvm-as < %s | \
; RUN:   llc -mtriple=i686-pc-linux-gnu -o %t -f
; RUN: grep -c set %t   | grep 5
; RUN: grep -c globl %t | grep 4
; RUN: grep -c weak %t  | grep 1

@bar = external global i32
@foo1 = alias i32* @bar
@foo2 = alias i32* @bar

%FunTy = type i32()

declare i32 @foo_f()
@bar_f = alias weak %FunTy* @foo_f

@bar_i = alias internal i32* @bar

@A = alias bitcast (i32* @bar to i64*)

define i32 @test() {
entry:
   %tmp = load i32* @foo1
   %tmp1 = load i32* @foo2
   %tmp0 = load i32* @bar_i
   %tmp2 = call i32 @foo_f()
   %tmp3 = add i32 %tmp, %tmp2
   %tmp4 = call %FunTy* @bar_f()
   %tmp5 = add i32 %tmp3, %tmp4
   %tmp6 = add i32 %tmp1, %tmp5
   %tmp7 = add i32 %tmp6, %tmp0
   ret i32 %tmp7
}
