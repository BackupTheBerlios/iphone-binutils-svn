; RUN: llvm-upgrade < %s | llvm-as | llc -march=x86 -mattr=-sse2,-sse3 | grep fchs


double %T() {
	ret double -1.0   ;; codegen as fld1/fchs, not as a load from cst pool
}
