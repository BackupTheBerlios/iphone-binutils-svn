; RUN: llvm-upgrade < %s | llvm-as | opt -deadargelim -disable-output

implementation

internal csretcc void %build_delaunay({int}* %agg.result) {
	ret void
}

void %test() {
  call csretcc void %build_delaunay({int}* null)
  ret void
}

