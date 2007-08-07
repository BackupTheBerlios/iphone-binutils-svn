//===-- ARMMachOWriterInfo.h - Mach-O Writer Info for ARM ---*- C++ -*-===//
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

#ifndef ARM_MACHO_WRITER_INFO_H
#define ARM_MACHO_WRITER_INFO_H

#include "llvm/Target/TargetMachOWriterInfo.h"

namespace llvm {

  // Forward declarations
  class MachineRelocation;
  class OutputBuffer;
  class ARMTargetMachine;

  class ARMMachOWriterInfo : public TargetMachOWriterInfo {
  public:
    ARMMachOWriterInfo(const ARMTargetMachine &TM);
    virtual ~ARMMachOWriterInfo();

  };

} // end llvm namespace

#endif // ARM_MACHO_WRITER_INFO_H


