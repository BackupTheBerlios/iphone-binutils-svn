//===- SparcInstrInfo.h - Sparc Instruction Information ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sparc implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef SPARCINSTRUCTIONINFO_H
#define SPARCINSTRUCTIONINFO_H

#include "llvm/Target/TargetInstrInfo.h"
#include "SparcRegisterInfo.h"

namespace llvm {

/// SPII - This namespace holds all of the target specific flags that
/// instruction info tracks.
///
namespace SPII {
  enum {
    Pseudo = (1<<0),
    Load = (1<<1),
    Store = (1<<2),
    DelaySlot = (1<<3)
  };
}

class SparcInstrInfo : public TargetInstrInfo {
  const SparcRegisterInfo RI;
public:
  SparcInstrInfo(SparcSubtarget &ST);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  virtual const MRegisterInfo &getRegisterInfo() const { return RI; }

  /// Return true if the instruction is a register to register move and
  /// leave the source and dest operands in the passed parameters.
  ///
  virtual bool isMoveInstr(const MachineInstr &MI,
                           unsigned &SrcReg, unsigned &DstReg) const;
  
  /// isLoadFromStackSlot - If the specified machine instruction is a direct
  /// load from a stack slot, return the virtual or physical register number of
  /// the destination along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than loading from the stack slot.
  virtual unsigned isLoadFromStackSlot(MachineInstr *MI, int &FrameIndex) const;
  
  /// isStoreToStackSlot - If the specified machine instruction is a direct
  /// store to a stack slot, return the virtual or physical register number of
  /// the source reg along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than storing to the stack slot.
  virtual unsigned isStoreToStackSlot(MachineInstr *MI, int &FrameIndex) const;
  
  
  virtual void InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                            MachineBasicBlock *FBB,
                            const std::vector<MachineOperand> &Cond) const;
};

}

#endif
