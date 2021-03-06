//===- AlphaInstrInfo.cpp - Alpha Instruction Information -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Alpha implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "Alpha.h"
#include "AlphaInstrInfo.h"
#include "AlphaGenInstrInfo.inc"
#include "llvm/CodeGen/MachineInstrBuilder.h"
using namespace llvm;

AlphaInstrInfo::AlphaInstrInfo()
  : TargetInstrInfo(AlphaInsts, sizeof(AlphaInsts)/sizeof(AlphaInsts[0])),
    RI(*this) { }


bool AlphaInstrInfo::isMoveInstr(const MachineInstr& MI,
                                 unsigned& sourceReg,
                                 unsigned& destReg) const {
  MachineOpCode oc = MI.getOpcode();
  if (oc == Alpha::BISr   || 
      oc == Alpha::CPYSS  || 
      oc == Alpha::CPYST  ||
      oc == Alpha::CPYSSt || 
      oc == Alpha::CPYSTs) {
    // or r1, r2, r2 
    // cpys(s|t) r1 r2 r2
    assert(MI.getNumOperands() >= 3 &&
           MI.getOperand(0).isRegister() &&
           MI.getOperand(1).isRegister() &&
           MI.getOperand(2).isRegister() &&
           "invalid Alpha BIS instruction!");
    if (MI.getOperand(1).getReg() == MI.getOperand(2).getReg()) {
      sourceReg = MI.getOperand(1).getReg();
      destReg = MI.getOperand(0).getReg();
      return true;
    }
  }
  return false;
}

unsigned 
AlphaInstrInfo::isLoadFromStackSlot(MachineInstr *MI, int &FrameIndex) const {
  switch (MI->getOpcode()) {
  case Alpha::LDL:
  case Alpha::LDQ:
  case Alpha::LDBU:
  case Alpha::LDWU:
  case Alpha::LDS:
  case Alpha::LDT:
    if (MI->getOperand(1).isFrameIndex()) {
      FrameIndex = MI->getOperand(1).getFrameIndex();
      return MI->getOperand(0).getReg();
    }
    break;
  }
  return 0;
}

unsigned 
AlphaInstrInfo::isStoreToStackSlot(MachineInstr *MI, int &FrameIndex) const {
  switch (MI->getOpcode()) {
  case Alpha::STL:
  case Alpha::STQ:
  case Alpha::STB:
  case Alpha::STW:
  case Alpha::STS:
  case Alpha::STT:
    if (MI->getOperand(1).isFrameIndex()) {
      FrameIndex = MI->getOperand(1).getFrameIndex();
      return MI->getOperand(0).getReg();
    }
    break;
  }
  return 0;
}

static bool isAlphaIntCondCode(unsigned Opcode) {
  switch (Opcode) {
  case Alpha::BEQ: 
  case Alpha::BNE: 
  case Alpha::BGE: 
  case Alpha::BGT: 
  case Alpha::BLE: 
  case Alpha::BLT: 
  case Alpha::BLBC: 
  case Alpha::BLBS:
    return true;
  default:
    return false;
  }
}

void AlphaInstrInfo::InsertBranch(MachineBasicBlock &MBB,MachineBasicBlock *TBB,
                                  MachineBasicBlock *FBB,
                                  const std::vector<MachineOperand> &Cond)const{
  assert(TBB && "InsertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 2 || Cond.size() == 0) && 
         "Alpha branch conditions have two components!");

  // One-way branch.
  if (FBB == 0) {
    if (Cond.empty())   // Unconditional branch
      BuildMI(&MBB, get(Alpha::BR)).addMBB(TBB);
    else                // Conditional branch
      if (isAlphaIntCondCode(Cond[0].getImm()))
        BuildMI(&MBB, get(Alpha::COND_BRANCH_I))
          .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
      else
        BuildMI(&MBB, get(Alpha::COND_BRANCH_F))
          .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
    return;
  }
  
  // Two-way Conditional Branch.
  if (isAlphaIntCondCode(Cond[0].getImm()))
    BuildMI(&MBB, get(Alpha::COND_BRANCH_I))
      .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
  else
    BuildMI(&MBB, get(Alpha::COND_BRANCH_F))
      .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
  BuildMI(&MBB, get(Alpha::BR)).addMBB(FBB);
}

static unsigned AlphaRevCondCode(unsigned Opcode) {
  switch (Opcode) {
  case Alpha::BEQ: return Alpha::BNE;
  case Alpha::BNE: return Alpha::BEQ;
  case Alpha::BGE: return Alpha::BLT;
  case Alpha::BGT: return Alpha::BLE;
  case Alpha::BLE: return Alpha::BGT;
  case Alpha::BLT: return Alpha::BGE;
  case Alpha::BLBC: return Alpha::BLBS;
  case Alpha::BLBS: return Alpha::BLBC;
  case Alpha::FBEQ: return Alpha::FBNE;
  case Alpha::FBNE: return Alpha::FBEQ;
  case Alpha::FBGE: return Alpha::FBLT;
  case Alpha::FBGT: return Alpha::FBLE;
  case Alpha::FBLE: return Alpha::FBGT;
  case Alpha::FBLT: return Alpha::FBGE;
  default:
    assert(0 && "Unknown opcode");
  }
}

// Branch analysis.
bool AlphaInstrInfo::AnalyzeBranch(MachineBasicBlock &MBB,MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 std::vector<MachineOperand> &Cond) const {
  // If the block has no terminators, it just falls into the block after it.
  MachineBasicBlock::iterator I = MBB.end();
  if (I == MBB.begin() || !isTerminatorInstr((--I)->getOpcode()))
    return false;

  // Get the last instruction in the block.
  MachineInstr *LastInst = I;
  
  // If there is only one terminator instruction, process it.
  if (I == MBB.begin() || !isTerminatorInstr((--I)->getOpcode())) {
    if (LastInst->getOpcode() == Alpha::BR) {
      TBB = LastInst->getOperand(0).getMachineBasicBlock();
      return false;
    } else if (LastInst->getOpcode() == Alpha::COND_BRANCH_I ||
               LastInst->getOpcode() == Alpha::COND_BRANCH_F) {
      // Block ends with fall-through condbranch.
      TBB = LastInst->getOperand(2).getMachineBasicBlock();
      Cond.push_back(LastInst->getOperand(0));
      Cond.push_back(LastInst->getOperand(1));
      return false;
    }
    // Otherwise, don't know what this is.
    return true;
  }
  
  // Get the instruction before it if it's a terminator.
  MachineInstr *SecondLastInst = I;

  // If there are three terminators, we don't know what sort of block this is.
  if (SecondLastInst && I != MBB.begin() &&
      isTerminatorInstr((--I)->getOpcode()))
    return true;
  
  // If the block ends with Alpha::BR and Alpha::COND_BRANCH_*, handle it.
  if ((SecondLastInst->getOpcode() == Alpha::COND_BRANCH_I ||
      SecondLastInst->getOpcode() == Alpha::COND_BRANCH_F) && 
      LastInst->getOpcode() == Alpha::BR) {
    TBB =  SecondLastInst->getOperand(2).getMachineBasicBlock();
    Cond.push_back(SecondLastInst->getOperand(0));
    Cond.push_back(SecondLastInst->getOperand(1));
    FBB = LastInst->getOperand(0).getMachineBasicBlock();
    return false;
  }
  
  // Otherwise, can't handle this.
  return true;
}

void AlphaInstrInfo::RemoveBranch(MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator I = MBB.end();
  if (I == MBB.begin()) return;
  --I;
  if (I->getOpcode() != Alpha::BR && 
      I->getOpcode() != Alpha::COND_BRANCH_I &&
      I->getOpcode() != Alpha::COND_BRANCH_F)
    return;
  
  // Remove the branch.
  I->eraseFromParent();
  
  I = MBB.end();

  if (I == MBB.begin()) return;
  --I;
  if (I->getOpcode() != Alpha::COND_BRANCH_I && 
      I->getOpcode() != Alpha::COND_BRANCH_F)
    return;
  
  // Remove the branch.
  I->eraseFromParent();
}

void AlphaInstrInfo::insertNoop(MachineBasicBlock &MBB, 
                                MachineBasicBlock::iterator MI) const {
  BuildMI(MBB, MI, get(Alpha::BISr), Alpha::R31).addReg(Alpha::R31)
    .addReg(Alpha::R31);
}

bool AlphaInstrInfo::BlockHasNoFallThrough(MachineBasicBlock &MBB) const {
  if (MBB.empty()) return false;
  
  switch (MBB.back().getOpcode()) {
  case Alpha::BR:     // Uncond branch.
  case Alpha::JMP:  // Indirect branch.
    return true;
  default: return false;
  }
}
bool AlphaInstrInfo::
ReverseBranchCondition(std::vector<MachineOperand> &Cond) const {
  assert(Cond.size() == 2 && "Invalid Alpha branch opcode!");
  Cond[0].setImm(AlphaRevCondCode(Cond[0].getImm()));
  return false;
}

