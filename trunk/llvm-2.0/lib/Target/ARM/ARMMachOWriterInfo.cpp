//===-- ARMMachOWriterInfo.cpp - Mach-O Writer Info for the ARM -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Bill Wendling and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements Mach-O writer information for the ARM backend.
//
//===----------------------------------------------------------------------===//

#include "ARMMachOWriterInfo.h"
#include "ARMTargetMachine.h"
#include "llvm/CodeGen/MachORelocation.h"
#include "llvm/Support/OutputBuffer.h"

using namespace llvm;

ARMMachOWriterInfo::ARMMachOWriterInfo(const ARMTargetMachine &TM)
  : TargetMachOWriterInfo(HDR_CPU_TYPE_ARM, HDR_CPU_SUBTYPE_ARM_ALL) {}
ARMMachOWriterInfo::~ARMMachOWriterInfo() {}

