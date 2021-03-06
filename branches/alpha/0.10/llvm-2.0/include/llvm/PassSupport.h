//===- llvm/PassSupport.h - Pass Support code -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines stuff that is used to define and "use" Passes.  This file
// is automatically #included by Pass.h, so:
//
//           NO .CPP FILES SHOULD INCLUDE THIS FILE DIRECTLY
//
// Instead, #include Pass.h.
//
// This file defines Pass registration code and classes used for it.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_PASS_SUPPORT_H
#define LLVM_PASS_SUPPORT_H

#include "llvm/System/IncludeFile.h"
// No need to include Pass.h, we are being included by it!

namespace llvm {

class TargetMachine;

//===---------------------------------------------------------------------------
/// PassInfo class - An instance of this class exists for every pass known by
/// the system, and can be obtained from a live Pass by calling its
/// getPassInfo() method.  These objects are set up by the RegisterPass<>
/// template, defined below.
///
class PassInfo {
  const char           *PassName;      // Nice name for Pass
  const char           *PassArgument;  // Command Line argument to run this pass
  intptr_t             PassID;      
  bool IsCFGOnlyPass;                  // Pass only looks at the CFG.
  bool IsAnalysisGroup;                // True if an analysis group.
  std::vector<const PassInfo*> ItfImpl;// Interfaces implemented by this pass

  Pass *(*NormalCtor)();

public:
  /// PassInfo ctor - Do not call this directly, this should only be invoked
  /// through RegisterPass.
  PassInfo(const char *name, const char *arg, intptr_t pi,
           Pass *(*normal)() = 0, bool isCFGOnly = false)
    : PassName(name), PassArgument(arg), PassID(pi), 
      IsCFGOnlyPass(isCFGOnly), IsAnalysisGroup(false), NormalCtor(normal) {
  }

  /// getPassName - Return the friendly name for the pass, never returns null
  ///
  const char *getPassName() const { return PassName; }
  void setPassName(const char *Name) { PassName = Name; }

  /// getPassArgument - Return the command line option that may be passed to
  /// 'opt' that will cause this pass to be run.  This will return null if there
  /// is no argument.
  ///
  const char *getPassArgument() const { return PassArgument; }

  /// getTypeInfo - Return the type_info object for the pass...
  /// TODO : Rename
  intptr_t getTypeInfo() const { return PassID; }

  /// isAnalysisGroup - Return true if this is an analysis group, not a normal
  /// pass.
  ///
  bool isAnalysisGroup() const { return IsAnalysisGroup; }
  void SetIsAnalysisGroup() { IsAnalysisGroup = true; }

  /// isCFGOnlyPass - return true if this pass only looks at the CFG for the
  /// function.
  bool isCFGOnlyPass() const { return IsCFGOnlyPass; }
  
  /// getNormalCtor - Return a pointer to a function, that when called, creates
  /// an instance of the pass and returns it.  This pointer may be null if there
  /// is no default constructor for the pass.
  ///
  Pass *(*getNormalCtor() const)() {
    return NormalCtor;
  }
  void setNormalCtor(Pass *(*Ctor)()) {
    NormalCtor = Ctor;
  }

  /// createPass() - Use this method to create an instance of this pass.
  Pass *createPass() const {
    assert((!isAnalysisGroup() || NormalCtor) &&
           "No default implementation found for analysis group!");
    assert(NormalCtor &&
           "Cannot call createPass on PassInfo without default ctor!");
    return NormalCtor();
  }

  /// addInterfaceImplemented - This method is called when this pass is
  /// registered as a member of an analysis group with the RegisterAnalysisGroup
  /// template.
  ///
  void addInterfaceImplemented(const PassInfo *ItfPI) {
    ItfImpl.push_back(ItfPI);
  }

  /// getInterfacesImplemented - Return a list of all of the analysis group
  /// interfaces implemented by this pass.
  ///
  const std::vector<const PassInfo*> &getInterfacesImplemented() const {
    return ItfImpl;
  }
};


//===---------------------------------------------------------------------------
/// RegisterPass<t> template - This template class is used to notify the system
/// that a Pass is available for use, and registers it into the internal
/// database maintained by the PassManager.  Unless this template is used, opt,
/// for example will not be able to see the pass and attempts to create the pass
/// will fail. This template is used in the follow manner (at global scope, in
/// your .cpp file):
///
/// static RegisterPass<YourPassClassName> tmp("passopt", "My Pass Name");
///
/// This statement will cause your pass to be created by calling the default
/// constructor exposed by the pass.  If you have a different constructor that
/// must be called, create a global constructor function (which takes the
/// arguments you need and returns a Pass*) and register your pass like this:
///
/// static RegisterPass<PassClassName> tmp("passopt", "My Name");
///
struct RegisterPassBase {
  /// getPassInfo - Get the pass info for the registered class...
  ///
  const PassInfo *getPassInfo() const { return &PIObj; }

  typedef Pass* (*NormalCtor_t)();
  
  RegisterPassBase(const char *Name, const char *Arg, intptr_t TI,
                   NormalCtor_t NormalCtor = 0, bool CFGOnly = false)
    : PIObj(Name, Arg, TI, NormalCtor, CFGOnly) {
    registerPass();
  }
  RegisterPassBase(intptr_t TI)
    : PIObj("", "", TI) {
    // This ctor may only be used for analysis groups: it does not auto-register
    // the pass.
    PIObj.SetIsAnalysisGroup();
  }

protected:
  PassInfo PIObj;       // The PassInfo object for this pass
  void registerPass();
  void unregisterPass();
};

template<typename PassName>
Pass *callDefaultCtor() { return new PassName(); }

template<typename PassName>
struct RegisterPass : public RegisterPassBase {

  // Register Pass using default constructor...
  RegisterPass(const char *PassArg, const char *Name, bool CFGOnly = false)
    : RegisterPassBase(Name, PassArg, (intptr_t)&PassName::ID,
                     (RegisterPassBase::NormalCtor_t)callDefaultCtor<PassName>, CFGOnly) {
  }
};


/// RegisterAnalysisGroup - Register a Pass as a member of an analysis _group_.
/// Analysis groups are used to define an interface (which need not derive from
/// Pass) that is required by passes to do their job.  Analysis Groups differ
/// from normal analyses because any available implementation of the group will
/// be used if it is available.
///
/// If no analysis implementing the interface is available, a default
/// implementation is created and added.  A pass registers itself as the default
/// implementation by specifying 'true' as the third template argument of this
/// class.
///
/// In addition to registering itself as an analysis group member, a pass must
/// register itself normally as well.  Passes may be members of multiple groups
/// and may still be "required" specifically by name.
///
/// The actual interface may also be registered as well (by not specifying the
/// second template argument).  The interface should be registered to associate
/// a nice name with the interface.
///
class RegisterAGBase : public RegisterPassBase {
  PassInfo *InterfaceInfo;
  const PassInfo *ImplementationInfo;
  bool isDefaultImplementation;
protected:
  explicit RegisterAGBase(intptr_t InterfaceID,
                          intptr_t PassID = 0,
                          bool isDefault = false);
  void setGroupName(const char *Name);
};

template<typename Interface, bool Default = false>
struct RegisterAnalysisGroup : public RegisterAGBase {
  explicit RegisterAnalysisGroup(RegisterPassBase &RPB)
    : RegisterAGBase((intptr_t) &Interface::ID, RPB.getPassInfo()->getTypeInfo(),
                     Default) {
  }

  explicit RegisterAnalysisGroup(const char *Name)
    : RegisterAGBase((intptr_t) &Interface::ID) {
    setGroupName(Name);
  }
};



//===---------------------------------------------------------------------------
/// PassRegistrationListener class - This class is meant to be derived from by
/// clients that are interested in which passes get registered and unregistered
/// at runtime (which can be because of the RegisterPass constructors being run
/// as the program starts up, or may be because a shared object just got
/// loaded).  Deriving from the PassRegistationListener class automatically
/// registers your object to receive callbacks indicating when passes are loaded
/// and removed.
///
struct PassRegistrationListener {

  /// PassRegistrationListener ctor - Add the current object to the list of
  /// PassRegistrationListeners...
  PassRegistrationListener();

  /// dtor - Remove object from list of listeners...
  ///
  virtual ~PassRegistrationListener();

  /// Callback functions - These functions are invoked whenever a pass is loaded
  /// or removed from the current executable.
  ///
  virtual void passRegistered(const PassInfo *P) {}

  /// enumeratePasses - Iterate over the registered passes, calling the
  /// passEnumerate callback on each PassInfo object.
  ///
  void enumeratePasses();

  /// passEnumerate - Callback function invoked when someone calls
  /// enumeratePasses on this PassRegistrationListener object.
  ///
  virtual void passEnumerate(const PassInfo *P) {}
};


} // End llvm namespace

#endif
