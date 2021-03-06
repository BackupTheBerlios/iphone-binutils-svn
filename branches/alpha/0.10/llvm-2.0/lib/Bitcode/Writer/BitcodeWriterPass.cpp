//===--- Bitcode/Writer/BitcodeWriterPass.cpp - Bitcode Writer ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Chris Lattner and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// BitcodeWriterPass implementation.
//
//===----------------------------------------------------------------------===//

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Pass.h"
using namespace llvm;

namespace {
  class WriteBitcodePass : public ModulePass {
    std::ostream *Out;                 // ostream to print on
  public:
    static char ID; // Pass identifcation, replacement for typeid
    WriteBitcodePass() : ModulePass((intptr_t) &ID), Out(0) { } 
    WriteBitcodePass(std::ostream &o) : ModulePass((intptr_t) &ID), Out(&o) {}
    
    bool runOnModule(Module &M) {
      if (Out)
        WriteBitcodeToFile(&M, *Out);
      return false;
    }
  };
}

char WriteBitcodePass::ID = 0;
static RegisterPass<WriteBitcodePass> X("emitbitcode", "Bitcode Writer");

/// CreateBitcodeWriterPass - Create and return a pass that writes the module
/// to the specified ostream.
ModulePass *llvm::CreateBitcodeWriterPass(std::ostream &Str) {
  return new WriteBitcodePass(Str);
}


