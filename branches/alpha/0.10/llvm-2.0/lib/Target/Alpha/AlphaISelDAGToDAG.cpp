//===-- AlphaISelDAGToDAG.cpp - Alpha pattern matching inst selector ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Andrew Lenharth and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines a pattern matching instruction selector for Alpha,
// converting from a legalized dag to a Alpha dag.
//
//===----------------------------------------------------------------------===//

#include "Alpha.h"
#include "AlphaTargetMachine.h"
#include "AlphaISelLowering.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SSARegMap.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/GlobalValue.h"
#include "llvm/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include <algorithm>
#include <queue>
#include <set>
using namespace llvm;

namespace {

  //===--------------------------------------------------------------------===//
  /// AlphaDAGToDAGISel - Alpha specific code to select Alpha machine
  /// instructions for SelectionDAG operations.
  class AlphaDAGToDAGISel : public SelectionDAGISel {
    AlphaTargetLowering AlphaLowering;

    static const int64_t IMM_LOW  = -32768;
    static const int64_t IMM_HIGH = 32767;
    static const int64_t IMM_MULT = 65536;
    static const int64_t IMM_FULLHIGH = IMM_HIGH + IMM_HIGH * IMM_MULT;
    static const int64_t IMM_FULLLOW = IMM_LOW + IMM_LOW  * IMM_MULT;

    static int64_t get_ldah16(int64_t x) {
      int64_t y = x / IMM_MULT;
      if (x % IMM_MULT > IMM_HIGH)
        ++y;
      return y;
    }

    static int64_t get_lda16(int64_t x) {
      return x - get_ldah16(x) * IMM_MULT;
    }

    /// get_zapImm - Return a zap mask if X is a valid immediate for a zapnot
    /// instruction (if not, return 0).  Note that this code accepts partial
    /// zap masks.  For example (and LHS, 1) is a valid zap, as long we know
    /// that the bits 1-7 of LHS are already zero.  If LHS is non-null, we are
    /// in checking mode.  If LHS is null, we assume that the mask has already
    /// been validated before.
    uint64_t get_zapImm(SDOperand LHS, uint64_t Constant) {
      uint64_t BitsToCheck = 0;
      unsigned Result = 0;
      for (unsigned i = 0; i != 8; ++i) {
        if (((Constant >> 8*i) & 0xFF) == 0) {
          // nothing to do.
        } else {
          Result |= 1 << i;
          if (((Constant >> 8*i) & 0xFF) == 0xFF) {
            // If the entire byte is set, zapnot the byte.
          } else if (LHS.Val == 0) {
            // Otherwise, if the mask was previously validated, we know its okay
            // to zapnot this entire byte even though all the bits aren't set.
          } else {
            // Otherwise we don't know that the it's okay to zapnot this entire
            // byte.  Only do this iff we can prove that the missing bits are
            // already null, so the bytezap doesn't need to really null them.
            BitsToCheck |= ~Constant & (0xFF << 8*i);
          }
        }
      }
      
      // If there are missing bits in a byte (for example, X & 0xEF00), check to
      // see if the missing bits (0x1000) are already known zero if not, the zap
      // isn't okay to do, as it won't clear all the required bits.
      if (BitsToCheck &&
          !getTargetLowering().MaskedValueIsZero(LHS, BitsToCheck))
        return 0;
      
      return Result;
    }
    
    static uint64_t get_zapImm(uint64_t x) {
      unsigned build = 0;
      for(int i = 0; i != 8; ++i) {
        if ((x & 0x00FF) == 0x00FF)
          build |= 1 << i;
        else if ((x & 0x00FF) != 0)
          return 0;
        x >>= 8;
      }
      return build;
    }
      
    
    static uint64_t getNearPower2(uint64_t x) {
      if (!x) return 0;
      unsigned at = CountLeadingZeros_64(x);
      uint64_t complow = 1 << (63 - at);
      uint64_t comphigh = 1 << (64 - at);
      //cerr << x << ":" << complow << ":" << comphigh << "\n";
      if (abs(complow - x) <= abs(comphigh - x))
        return complow;
      else
        return comphigh;
    }

    static bool chkRemNearPower2(uint64_t x, uint64_t r, bool swap) {
      uint64_t y = getNearPower2(x);
      if (swap)
        return (y - x) == r;
      else
        return (x - y) == r;
    }

    static bool isFPZ(SDOperand N) {
      ConstantFPSDNode *CN = dyn_cast<ConstantFPSDNode>(N);
      return (CN && (CN->isExactlyValue(+0.0) || CN->isExactlyValue(-0.0)));
    }
    static bool isFPZn(SDOperand N) {
      ConstantFPSDNode *CN = dyn_cast<ConstantFPSDNode>(N);
      return (CN && CN->isExactlyValue(-0.0));
    }
    static bool isFPZp(SDOperand N) {
      ConstantFPSDNode *CN = dyn_cast<ConstantFPSDNode>(N);
      return (CN && CN->isExactlyValue(+0.0));
    }

  public:
    AlphaDAGToDAGISel(TargetMachine &TM)
      : SelectionDAGISel(AlphaLowering), 
        AlphaLowering(*(AlphaTargetLowering*)(TM.getTargetLowering())) 
    {}

    /// getI64Imm - Return a target constant with the specified value, of type
    /// i64.
    inline SDOperand getI64Imm(int64_t Imm) {
      return CurDAG->getTargetConstant(Imm, MVT::i64);
    }

    // Select - Convert the specified operand from a target-independent to a
    // target-specific node if it hasn't already been changed.
    SDNode *Select(SDOperand Op);
    
    /// InstructionSelectBasicBlock - This callback is invoked by
    /// SelectionDAGISel when it has created a SelectionDAG for us to codegen.
    virtual void InstructionSelectBasicBlock(SelectionDAG &DAG);
    
    virtual const char *getPassName() const {
      return "Alpha DAG->DAG Pattern Instruction Selection";
    } 

    /// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
    /// inline asm expressions.
    virtual bool SelectInlineAsmMemoryOperand(const SDOperand &Op,
                                              char ConstraintCode,
                                              std::vector<SDOperand> &OutOps,
                                              SelectionDAG &DAG) {
      SDOperand Op0;
      switch (ConstraintCode) {
      default: return true;
      case 'm':   // memory
        Op0 = Op;
        AddToISelQueue(Op0);
        break;
      }
      
      OutOps.push_back(Op0);
      return false;
    }
    
// Include the pieces autogenerated from the target description.
#include "AlphaGenDAGISel.inc"
    
private:
    SDOperand getGlobalBaseReg();
    SDOperand getGlobalRetAddr();
    void SelectCALL(SDOperand Op);

  };
}

/// getGlobalBaseReg - Output the instructions required to put the
/// GOT address into a register.
///
SDOperand AlphaDAGToDAGISel::getGlobalBaseReg() {
  MachineFunction* MF = BB->getParent();
  unsigned GP = 0;
  for(MachineFunction::livein_iterator ii = MF->livein_begin(), 
        ee = MF->livein_end(); ii != ee; ++ii)
    if (ii->first == Alpha::R29) {
      GP = ii->second;
      break;
    }
  assert(GP && "GOT PTR not in liveins");
  return CurDAG->getCopyFromReg(CurDAG->getEntryNode(), 
                                GP, MVT::i64);
}

/// getRASaveReg - Grab the return address
///
SDOperand AlphaDAGToDAGISel::getGlobalRetAddr() {
  MachineFunction* MF = BB->getParent();
  unsigned RA = 0;
  for(MachineFunction::livein_iterator ii = MF->livein_begin(), 
        ee = MF->livein_end(); ii != ee; ++ii)
    if (ii->first == Alpha::R26) {
      RA = ii->second;
      break;
    }
  assert(RA && "RA PTR not in liveins");
  return CurDAG->getCopyFromReg(CurDAG->getEntryNode(),
                                RA, MVT::i64);
}

/// InstructionSelectBasicBlock - This callback is invoked by
/// SelectionDAGISel when it has created a SelectionDAG for us to codegen.
void AlphaDAGToDAGISel::InstructionSelectBasicBlock(SelectionDAG &DAG) {
  DEBUG(BB->dump());
  
  // Select target instructions for the DAG.
  DAG.setRoot(SelectRoot(DAG.getRoot()));
  DAG.RemoveDeadNodes();
  
  // Emit machine code to BB. 
  ScheduleAndEmitDAG(DAG);
}

// Select - Convert the specified operand from a target-independent to a
// target-specific node if it hasn't already been changed.
SDNode *AlphaDAGToDAGISel::Select(SDOperand Op) {
  SDNode *N = Op.Val;
  if (N->getOpcode() >= ISD::BUILTIN_OP_END &&
      N->getOpcode() < AlphaISD::FIRST_NUMBER) {
    return NULL;   // Already selected.
  }

  switch (N->getOpcode()) {
  default: break;
  case AlphaISD::CALL:
    SelectCALL(Op);
    return NULL;

  case ISD::FrameIndex: {
    int FI = cast<FrameIndexSDNode>(N)->getIndex();
    return CurDAG->SelectNodeTo(N, Alpha::LDA, MVT::i64,
                                CurDAG->getTargetFrameIndex(FI, MVT::i32),
                                getI64Imm(0));
  }
  case ISD::GLOBAL_OFFSET_TABLE: {
    SDOperand Result = getGlobalBaseReg();
    ReplaceUses(Op, Result);
    return NULL;
  }
  case AlphaISD::GlobalRetAddr: {
    SDOperand Result = getGlobalRetAddr();
    ReplaceUses(Op, Result);
    return NULL;
  }
  
  case AlphaISD::DivCall: {
    SDOperand Chain = CurDAG->getEntryNode();
    SDOperand N0 = Op.getOperand(0);
    SDOperand N1 = Op.getOperand(1);
    SDOperand N2 = Op.getOperand(2);
    AddToISelQueue(N0);
    AddToISelQueue(N1);
    AddToISelQueue(N2);
    Chain = CurDAG->getCopyToReg(Chain, Alpha::R24, N1, 
                                 SDOperand(0,0));
    Chain = CurDAG->getCopyToReg(Chain, Alpha::R25, N2, 
                                 Chain.getValue(1));
    Chain = CurDAG->getCopyToReg(Chain, Alpha::R27, N0, 
                                 Chain.getValue(1));
    SDNode *CNode =
      CurDAG->getTargetNode(Alpha::JSRs, MVT::Other, MVT::Flag, 
                            Chain, Chain.getValue(1));
    Chain = CurDAG->getCopyFromReg(Chain, Alpha::R27, MVT::i64, 
                                   SDOperand(CNode, 1));
    return CurDAG->SelectNodeTo(N, Alpha::BISr, MVT::i64, Chain, Chain);
  }

  case ISD::READCYCLECOUNTER: {
    SDOperand Chain = N->getOperand(0);
    AddToISelQueue(Chain); //Select chain
    return CurDAG->getTargetNode(Alpha::RPCC, MVT::i64, MVT::Other,
                                 Chain);
  }

  case ISD::Constant: {
    uint64_t uval = cast<ConstantSDNode>(N)->getValue();
    
    if (uval == 0) {
      SDOperand Result = CurDAG->getCopyFromReg(CurDAG->getEntryNode(),
                                                Alpha::R31, MVT::i64);
      ReplaceUses(Op, Result);
      return NULL;
    }

    int64_t val = (int64_t)uval;
    int32_t val32 = (int32_t)val;
    if (val <= IMM_HIGH + IMM_HIGH * IMM_MULT &&
        val >= IMM_LOW  + IMM_LOW  * IMM_MULT)
      break; //(LDAH (LDA))
    if ((uval >> 32) == 0 && //empty upper bits
        val32 <= IMM_HIGH + IMM_HIGH * IMM_MULT)
      // val32 >= IMM_LOW  + IMM_LOW  * IMM_MULT) //always true
      break; //(zext (LDAH (LDA)))
    //Else use the constant pool
    ConstantInt *C = ConstantInt::get(Type::Int64Ty, uval);
    SDOperand CPI = CurDAG->getTargetConstantPool(C, MVT::i64);
    SDNode *Tmp = CurDAG->getTargetNode(Alpha::LDAHr, MVT::i64, CPI,
                                        getGlobalBaseReg());
    return CurDAG->SelectNodeTo(N, Alpha::LDQr, MVT::i64, MVT::Other, 
                                CPI, SDOperand(Tmp, 0), CurDAG->getEntryNode());
  }
  case ISD::TargetConstantFP: {
    ConstantFPSDNode *CN = cast<ConstantFPSDNode>(N);
    bool isDouble = N->getValueType(0) == MVT::f64;
    MVT::ValueType T = isDouble ? MVT::f64 : MVT::f32;
    if (CN->isExactlyValue(+0.0)) {
      return CurDAG->SelectNodeTo(N, isDouble ? Alpha::CPYST : Alpha::CPYSS,
                                  T, CurDAG->getRegister(Alpha::F31, T),
                                  CurDAG->getRegister(Alpha::F31, T));
    } else if ( CN->isExactlyValue(-0.0)) {
      return CurDAG->SelectNodeTo(N, isDouble ? Alpha::CPYSNT : Alpha::CPYSNS,
                                  T, CurDAG->getRegister(Alpha::F31, T),
                                  CurDAG->getRegister(Alpha::F31, T));
    } else {
      abort();
    }
    break;
  }

  case ISD::SETCC:
    if (MVT::isFloatingPoint(N->getOperand(0).Val->getValueType(0))) {
      ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(2))->get();

      unsigned Opc = Alpha::WTF;
      bool rev = false;
      bool inv = false;
      switch(CC) {
      default: DEBUG(N->dump()); assert(0 && "Unknown FP comparison!");
      case ISD::SETEQ: case ISD::SETOEQ: case ISD::SETUEQ:
        Opc = Alpha::CMPTEQ; break;
      case ISD::SETLT: case ISD::SETOLT: case ISD::SETULT: 
        Opc = Alpha::CMPTLT; break;
      case ISD::SETLE: case ISD::SETOLE: case ISD::SETULE: 
        Opc = Alpha::CMPTLE; break;
      case ISD::SETGT: case ISD::SETOGT: case ISD::SETUGT: 
        Opc = Alpha::CMPTLT; rev = true; break;
      case ISD::SETGE: case ISD::SETOGE: case ISD::SETUGE: 
        Opc = Alpha::CMPTLE; rev = true; break;
      case ISD::SETNE: case ISD::SETONE: case ISD::SETUNE:
        Opc = Alpha::CMPTEQ; inv = true; break;
      case ISD::SETO:
        Opc = Alpha::CMPTUN; inv = true; break;
      case ISD::SETUO:
        Opc = Alpha::CMPTUN; break;
      };
      SDOperand tmp1 = N->getOperand(rev?1:0);
      SDOperand tmp2 = N->getOperand(rev?0:1);
      AddToISelQueue(tmp1);
      AddToISelQueue(tmp2);
      SDNode *cmp = CurDAG->getTargetNode(Opc, MVT::f64, tmp1, tmp2);
      if (inv) 
        cmp = CurDAG->getTargetNode(Alpha::CMPTEQ, MVT::f64, SDOperand(cmp, 0), 
                                    CurDAG->getRegister(Alpha::F31, MVT::f64));
      switch(CC) {
      case ISD::SETUEQ: case ISD::SETULT: case ISD::SETULE:
      case ISD::SETUNE: case ISD::SETUGT: case ISD::SETUGE:
       {
         SDNode* cmp2 = CurDAG->getTargetNode(Alpha::CMPTUN, MVT::f64,
                                              tmp1, tmp2);
         cmp = CurDAG->getTargetNode(Alpha::ADDT, MVT::f64, 
                                     SDOperand(cmp2, 0), SDOperand(cmp, 0));
         break;
       }
      default: break;
      }

      SDNode* LD = CurDAG->getTargetNode(Alpha::FTOIT, MVT::i64, SDOperand(cmp, 0));
      return CurDAG->getTargetNode(Alpha::CMPULT, MVT::i64, 
                                   CurDAG->getRegister(Alpha::R31, MVT::i64),
                                   SDOperand(LD,0));
    }
    break;

  case ISD::SELECT:
    if (MVT::isFloatingPoint(N->getValueType(0)) &&
        (N->getOperand(0).getOpcode() != ISD::SETCC ||
         !MVT::isFloatingPoint(N->getOperand(0).getOperand(1).getValueType()))) {
      //This should be the condition not covered by the Patterns
      //FIXME: Don't have SelectCode die, but rather return something testable
      // so that things like this can be caught in fall though code
      //move int to fp
      bool isDouble = N->getValueType(0) == MVT::f64;
      SDOperand cond = N->getOperand(0);
      SDOperand TV = N->getOperand(1);
      SDOperand FV = N->getOperand(2);
      AddToISelQueue(cond);
      AddToISelQueue(TV);
      AddToISelQueue(FV);
      
      SDNode* LD = CurDAG->getTargetNode(Alpha::ITOFT, MVT::f64, cond);
      return CurDAG->getTargetNode(isDouble?Alpha::FCMOVNET:Alpha::FCMOVNES,
                                   MVT::f64, FV, TV, SDOperand(LD,0));
    }
    break;

  case ISD::AND: {
    ConstantSDNode* SC = NULL;
    ConstantSDNode* MC = NULL;
    if (N->getOperand(0).getOpcode() == ISD::SRL &&
        (MC = dyn_cast<ConstantSDNode>(N->getOperand(1))) &&
        (SC = dyn_cast<ConstantSDNode>(N->getOperand(0).getOperand(1)))) {
      uint64_t sval = SC->getValue();
      uint64_t mval = MC->getValue();
      // If the result is a zap, let the autogened stuff handle it.
      if (get_zapImm(N->getOperand(0), mval))
        break;
      // given mask X, and shift S, we want to see if there is any zap in the
      // mask if we play around with the botton S bits
      uint64_t dontcare = (~0ULL) >> (64 - sval);
      uint64_t mask = mval << sval;
      
      if (get_zapImm(mask | dontcare))
        mask = mask | dontcare;
      
      if (get_zapImm(mask)) {
        AddToISelQueue(N->getOperand(0).getOperand(0));
        SDOperand Z = 
          SDOperand(CurDAG->getTargetNode(Alpha::ZAPNOTi, MVT::i64,
                                          N->getOperand(0).getOperand(0),
                                          getI64Imm(get_zapImm(mask))), 0);
        return CurDAG->getTargetNode(Alpha::SRLr, MVT::i64, Z, 
                                     getI64Imm(sval));
      }
    }
    break;
  }

  }

  return SelectCode(Op);
}

void AlphaDAGToDAGISel::SelectCALL(SDOperand Op) {
  //TODO: add flag stuff to prevent nondeturministic breakage!

  SDNode *N = Op.Val;
  SDOperand Chain = N->getOperand(0);
  SDOperand Addr = N->getOperand(1);
  SDOperand InFlag(0,0);  // Null incoming flag value.
  AddToISelQueue(Chain);

   std::vector<SDOperand> CallOperands;
   std::vector<MVT::ValueType> TypeOperands;
  
   //grab the arguments
   for(int i = 2, e = N->getNumOperands(); i < e; ++i) {
     TypeOperands.push_back(N->getOperand(i).getValueType());
     AddToISelQueue(N->getOperand(i));
     CallOperands.push_back(N->getOperand(i));
   }
   int count = N->getNumOperands() - 2;

   static const unsigned args_int[] = {Alpha::R16, Alpha::R17, Alpha::R18,
                                       Alpha::R19, Alpha::R20, Alpha::R21};
   static const unsigned args_float[] = {Alpha::F16, Alpha::F17, Alpha::F18,
                                         Alpha::F19, Alpha::F20, Alpha::F21};
   
   for (int i = 6; i < count; ++i) {
     unsigned Opc = Alpha::WTF;
     if (MVT::isInteger(TypeOperands[i])) {
       Opc = Alpha::STQ;
     } else if (TypeOperands[i] == MVT::f32) {
       Opc = Alpha::STS;
     } else if (TypeOperands[i] == MVT::f64) {
       Opc = Alpha::STT;
     } else
       assert(0 && "Unknown operand"); 

     SDOperand Ops[] = { CallOperands[i],  getI64Imm((i - 6) * 8), 
                         CurDAG->getCopyFromReg(Chain, Alpha::R30, MVT::i64),
                         Chain };
     Chain = SDOperand(CurDAG->getTargetNode(Opc, MVT::Other, Ops, 4), 0);
   }
   for (int i = 0; i < std::min(6, count); ++i) {
     if (MVT::isInteger(TypeOperands[i])) {
       Chain = CurDAG->getCopyToReg(Chain, args_int[i], CallOperands[i], InFlag);
       InFlag = Chain.getValue(1);
     } else if (TypeOperands[i] == MVT::f32 || TypeOperands[i] == MVT::f64) {
       Chain = CurDAG->getCopyToReg(Chain, args_float[i], CallOperands[i], InFlag);
       InFlag = Chain.getValue(1);
     } else
       assert(0 && "Unknown operand"); 
   }

   // Finally, once everything is in registers to pass to the call, emit the
   // call itself.
   if (Addr.getOpcode() == AlphaISD::GPRelLo) {
     SDOperand GOT = getGlobalBaseReg();
     Chain = CurDAG->getCopyToReg(Chain, Alpha::R29, GOT, InFlag);
     InFlag = Chain.getValue(1);
     Chain = SDOperand(CurDAG->getTargetNode(Alpha::BSR, MVT::Other, MVT::Flag, 
                                             Addr.getOperand(0), Chain, InFlag), 0);
   } else {
     AddToISelQueue(Addr);
     Chain = CurDAG->getCopyToReg(Chain, Alpha::R27, Addr, InFlag);
     InFlag = Chain.getValue(1);
     Chain = SDOperand(CurDAG->getTargetNode(Alpha::JSR, MVT::Other, MVT::Flag, 
                                             Chain, InFlag), 0);
   }
   InFlag = Chain.getValue(1);

   std::vector<SDOperand> CallResults;
  
   switch (N->getValueType(0)) {
   default: assert(0 && "Unexpected ret value!");
     case MVT::Other: break;
   case MVT::i64:
     Chain = CurDAG->getCopyFromReg(Chain, Alpha::R0, MVT::i64, InFlag).getValue(1);
     CallResults.push_back(Chain.getValue(0));
     break;
   case MVT::f32:
     Chain = CurDAG->getCopyFromReg(Chain, Alpha::F0, MVT::f32, InFlag).getValue(1);
     CallResults.push_back(Chain.getValue(0));
     break;
   case MVT::f64:
     Chain = CurDAG->getCopyFromReg(Chain, Alpha::F0, MVT::f64, InFlag).getValue(1);
     CallResults.push_back(Chain.getValue(0));
     break;
   }

   CallResults.push_back(Chain);
   for (unsigned i = 0, e = CallResults.size(); i != e; ++i)
     ReplaceUses(Op.getValue(i), CallResults[i]);
}


/// createAlphaISelDag - This pass converts a legalized DAG into a 
/// Alpha-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createAlphaISelDag(TargetMachine &TM) {
  return new AlphaDAGToDAGISel(TM);
}
