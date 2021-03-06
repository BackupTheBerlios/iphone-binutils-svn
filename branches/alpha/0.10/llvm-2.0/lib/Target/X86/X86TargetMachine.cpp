//===-- X86TargetMachine.cpp - Define TargetMachine for the X86 -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the X86 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "X86TargetAsmInfo.h"
#include "X86TargetMachine.h"
#include "X86.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachineRegistry.h"
#include "llvm/Transforms/Scalar.h"
using namespace llvm;

/// X86TargetMachineModule - Note that this is used on hosts that cannot link
/// in a library unless there are references into the library.  In particular,
/// it seems that it is not possible to get things to work on Win32 without
/// this.  Though it is unused, do not remove it.
extern "C" int X86TargetMachineModule;
int X86TargetMachineModule = 0;

namespace {
  // Register the target.
  RegisterTarget<X86_32TargetMachine>
  X("x86",    "  32-bit X86: Pentium-Pro and above");
  RegisterTarget<X86_64TargetMachine>
  Y("x86-64", "  64-bit X86: EM64T and AMD64");
}

const TargetAsmInfo *X86TargetMachine::createTargetAsmInfo() const {
  return new X86TargetAsmInfo(*this);
}

unsigned X86_32TargetMachine::getJITMatchQuality() {
#if defined(i386) || defined(__i386__) || defined(__x86__) || defined(_M_IX86)
  return 10;
#endif
  return 0;
}

unsigned X86_64TargetMachine::getJITMatchQuality() {
#if defined(__x86_64__)
  return 10;
#endif
  return 0;
}

unsigned X86_32TargetMachine::getModuleMatchQuality(const Module &M) {
  // We strongly match "i[3-9]86-*".
  std::string TT = M.getTargetTriple();
  if (TT.size() >= 5 && TT[0] == 'i' && TT[2] == '8' && TT[3] == '6' &&
      TT[4] == '-' && TT[1] - '3' < 6)
    return 20;

  if (M.getEndianness()  == Module::LittleEndian &&
      M.getPointerSize() == Module::Pointer32)
    return 10;                                   // Weak match
  else if (M.getEndianness() != Module::AnyEndianness ||
           M.getPointerSize() != Module::AnyPointerSize)
    return 0;                                    // Match for some other target

  return getJITMatchQuality()/2;
}

unsigned X86_64TargetMachine::getModuleMatchQuality(const Module &M) {
  // We strongly match "x86_64-*".
  std::string TT = M.getTargetTriple();
  if (TT.size() >= 7 && TT[0] == 'x' && TT[1] == '8' && TT[2] == '6' &&
      TT[3] == '_' && TT[4] == '6' && TT[5] == '4' && TT[6] == '-')
    return 20;

  // We strongly match "amd64-*".
  if (TT.size() >= 6 && TT[0] == 'a' && TT[1] == 'm' && TT[2] == 'd' &&
      TT[3] == '6' && TT[4] == '4' && TT[5] == '-')
    return 20;
  
  if (M.getEndianness()  == Module::LittleEndian &&
      M.getPointerSize() == Module::Pointer64)
    return 10;                                   // Weak match
  else if (M.getEndianness() != Module::AnyEndianness ||
           M.getPointerSize() != Module::AnyPointerSize)
    return 0;                                    // Match for some other target

  return getJITMatchQuality()/2;
}

X86_32TargetMachine::X86_32TargetMachine(const Module &M, const std::string &FS) 
  : X86TargetMachine(M, FS, false) {
}


X86_64TargetMachine::X86_64TargetMachine(const Module &M, const std::string &FS)
  : X86TargetMachine(M, FS, true) {
}

/// X86TargetMachine ctor - Create an ILP32 architecture model
///
X86TargetMachine::X86TargetMachine(const Module &M, const std::string &FS,
                                   bool is64Bit)
  : Subtarget(M, FS, is64Bit),
    DataLayout(Subtarget.is64Bit() ?
               std::string("e-p:64:64-f64:32:64-i64:32:64") :
               std::string("e-p:32:32-f64:32:64-i64:32:64")),
    FrameInfo(TargetFrameInfo::StackGrowsDown,
              Subtarget.getStackAlignment(), Subtarget.is64Bit() ? -8 : -4),
    InstrInfo(*this), JITInfo(*this), TLInfo(*this) {
  if (getRelocationModel() == Reloc::Default)
    if (Subtarget.isTargetDarwin() || Subtarget.isTargetCygMing())
      setRelocationModel(Reloc::DynamicNoPIC);
    else
      setRelocationModel(Reloc::Static);
  if (Subtarget.is64Bit()) {
    // No DynamicNoPIC support under X86-64.
    if (getRelocationModel() == Reloc::DynamicNoPIC)
      setRelocationModel(Reloc::PIC_);
    // Default X86-64 code model is small.
    if (getCodeModel() == CodeModel::Default)
      setCodeModel(CodeModel::Small);
  }

  if (Subtarget.isTargetCygMing())
    Subtarget.setPICStyle(PICStyle::WinPIC);
  else if (Subtarget.isTargetDarwin())
    if (Subtarget.is64Bit())
      Subtarget.setPICStyle(PICStyle::RIPRel);
    else
      Subtarget.setPICStyle(PICStyle::Stub);
  else if (Subtarget.isTargetELF())
    if (Subtarget.is64Bit())
      Subtarget.setPICStyle(PICStyle::RIPRel);
    else
      Subtarget.setPICStyle(PICStyle::GOT);
}

//===----------------------------------------------------------------------===//
// Pass Pipeline Configuration
//===----------------------------------------------------------------------===//

bool X86TargetMachine::addInstSelector(FunctionPassManager &PM, bool Fast) {
  // Install an instruction selector.
  PM.add(createX86ISelDag(*this, Fast));
  return false;
}

bool X86TargetMachine::addPostRegAlloc(FunctionPassManager &PM, bool Fast) {
  PM.add(createX86FloatingPointStackifierPass());
  return true;  // -print-machineinstr should print after this.
}

bool X86TargetMachine::addAssemblyEmitter(FunctionPassManager &PM, bool Fast, 
                                          std::ostream &Out) {
  PM.add(createX86CodePrinterPass(Out, *this));
  return false;
}

bool X86TargetMachine::addCodeEmitter(FunctionPassManager &PM, bool Fast,
                                      MachineCodeEmitter &MCE) {
  // FIXME: Move this to TargetJITInfo!
  setRelocationModel(Reloc::Static);
  Subtarget.setPICStyle(PICStyle::None);
  
  // JIT cannot ensure globals are placed in the lower 4G of address.
  if (Subtarget.is64Bit())
    setCodeModel(CodeModel::Large);

  PM.add(createX86CodeEmitterPass(*this, MCE));
  return false;
}

bool X86TargetMachine::addSimpleCodeEmitter(FunctionPassManager &PM, bool Fast,
                                            MachineCodeEmitter &MCE) {
  PM.add(createX86CodeEmitterPass(*this, MCE));
  return false;
}
