//=====-- AlphaSubtarget.h - Define Subtarget for the Alpha --*- C++ -*--====//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Andrew Lenharth and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Alpha specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#ifndef ALPHASUBTARGET_H
#define ALPHASUBTARGET_H

#include "llvm/Target/TargetInstrItineraries.h"
#include "llvm/Target/TargetSubtarget.h"

#include <string>

namespace llvm {
class Module;

class AlphaSubtarget : public TargetSubtarget {
protected:

  bool HasCT;

  InstrItineraryData InstrItins;

public:
  /// This constructor initializes the data members to match that
  /// of the specified module.
  ///
  AlphaSubtarget(const Module &M, const std::string &FS);
  
  /// ParseSubtargetFeatures - Parses features string setting specified 
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(const std::string &FS, const std::string &CPU);

  bool hasCT() const { return HasCT; }
};
} // End llvm namespace

#endif
