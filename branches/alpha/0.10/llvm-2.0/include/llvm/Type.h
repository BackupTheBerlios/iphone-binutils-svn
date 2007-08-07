//===-- llvm/Type.h - Classes for handling data types -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef LLVM_TYPE_H
#define LLVM_TYPE_H

#include "llvm/AbstractTypeUser.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/Streams.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/ADT/iterator"
#include <string>
#include <vector>

namespace llvm {

class DerivedType;
class PointerType;
class IntegerType;
class TypeMapBase;

/// This file contains the declaration of the Type class.  For more "Type" type
/// stuff, look in DerivedTypes.h.
///
/// The instances of the Type class are immutable: once they are created,
/// they are never changed.  Also note that only one instance of a particular
/// type is ever created.  Thus seeing if two types are equal is a matter of
/// doing a trivial pointer comparison. To enforce that no two equal instances
/// are created, Type instances can only be created via static factory methods 
/// in class Type and in derived classes.
/// 
/// Once allocated, Types are never free'd, unless they are an abstract type
/// that is resolved to a more concrete type.
/// 
/// Types themself don't have a name, and can be named either by:
/// - using SymbolTable instance, typically from some Module,
/// - using convenience methods in the Module class (which uses module's 
///    SymbolTable too).
///
/// Opaque types are simple derived types with no state.  There may be many
/// different Opaque type objects floating around, but two are only considered
/// identical if they are pointer equals of each other.  This allows us to have
/// two opaque types that end up resolving to different concrete types later.
///
/// Opaque types are also kinda weird and scary and different because they have
/// to keep a list of uses of the type.  When, through linking, parsing, or
/// bytecode reading, they become resolved, they need to find and update all
/// users of the unknown type, causing them to reference a new, more concrete
/// type.  Opaque types are deleted when their use list dwindles to zero users.
///
/// @brief Root of type hierarchy
class Type : public AbstractTypeUser {
public:
  //===-------------------------------------------------------------------===//
  /// Definitions of all of the base types for the Type system.  Based on this
  /// value, you can cast to a "DerivedType" subclass (see DerivedTypes.h)
  /// Note: If you add an element to this, you need to add an element to the
  /// Type::getPrimitiveType function, or else things will break!
  ///
  enum TypeID {
    // PrimitiveTypes .. make sure LastPrimitiveTyID stays up to date
    VoidTyID = 0,    ///<  0: type with no size
    FloatTyID,       ///<  1: 32 bit floating point type
    DoubleTyID,      ///<  2: 64 bit floating point type
    LabelTyID,       ///<  3: Labels

    // Derived types... see DerivedTypes.h file...
    // Make sure FirstDerivedTyID stays up to date!!!
    IntegerTyID,     ///<  4: Arbitrary bit width integers
    FunctionTyID,    ///<  5: Functions
    StructTyID,      ///<  6: Structures
    PackedStructTyID,///<  7: Packed Structure. This is for bytecode only
    ArrayTyID,       ///<  8: Arrays
    PointerTyID,     ///<  9: Pointers
    OpaqueTyID,      ///< 10: Opaque: type with unknown structure
    VectorTyID,      ///< 11: SIMD 'packed' format, or other vector type

    NumTypeIDs,                         // Must remain as last defined ID
    LastPrimitiveTyID = LabelTyID,
    FirstDerivedTyID = IntegerTyID
  };

private:
  TypeID   ID : 8;    // The current base type of this type.
  bool     Abstract : 1;  // True if type contains an OpaqueType
  unsigned SubclassData : 23; //Space for subclasses to store data

  /// RefCount - This counts the number of PATypeHolders that are pointing to
  /// this type.  When this number falls to zero, if the type is abstract and
  /// has no AbstractTypeUsers, the type is deleted.  This is only sensical for
  /// derived types.
  ///
  mutable unsigned RefCount;

  const Type *getForwardedTypeInternal() const;

  // Some Type instances are allocated as arrays, some aren't. So we provide
  // this method to get the right kind of destruction for the type of Type.
  void destroy() const; // const is a lie, this does "delete this"!

protected:
  explicit Type(TypeID id) : ID(id), Abstract(false), SubclassData(0),
                             RefCount(0), ForwardType(0), NumContainedTys(0),
                             ContainedTys(0) {}
  virtual ~Type() {
    assert(AbstractTypeUsers.empty() && "Abstract types remain");
  }

  /// Types can become nonabstract later, if they are refined.
  ///
  inline void setAbstract(bool Val) { Abstract = Val; }

  unsigned getRefCount() const { return RefCount; }

  unsigned getSubclassData() const { return SubclassData; }
  void setSubclassData(unsigned val) { SubclassData = val; }

  /// ForwardType - This field is used to implement the union find scheme for
  /// abstract types.  When types are refined to other types, this field is set
  /// to the more refined type.  Only abstract types can be forwarded.
  mutable const Type *ForwardType;


  /// AbstractTypeUsers - Implement a list of the users that need to be notified
  /// if I am a type, and I get resolved into a more concrete type.
  ///
  mutable std::vector<AbstractTypeUser *> AbstractTypeUsers;

  /// NumContainedTys - Keeps track of how many PATypeHandle instances there
  /// are at the end of this type instance for the list of contained types. It
  /// is the subclasses responsibility to set this up. Set to 0 if there are no
  /// contained types in this type.
  unsigned NumContainedTys;

  /// ContainedTys - A pointer to the array of Types (PATypeHandle) contained 
  /// by this Type.  For example, this includes the arguments of a function 
  /// type, the elements of a structure, the pointee of a pointer, the element
  /// type of an array, etc.  This pointer may be 0 for types that don't 
  /// contain other types (Integer, Double, Float).  In general, the subclass 
  /// should arrange for space for the PATypeHandles to be included in the 
  /// allocation of the type object and set this pointer to the address of the 
  /// first element. This allows the Type class to manipulate the ContainedTys 
  /// without understanding the subclass's placement for this array.  keeping 
  /// it here also allows the subtype_* members to be implemented MUCH more 
  /// efficiently, and dynamically very few types do not contain any elements.
  PATypeHandle *ContainedTys;

public:
  void print(std::ostream &O) const;
  void print(std::ostream *O) const { if (O) print(*O); }

  /// @brief Debugging support: print to stderr
  void dump() const;

  //===--------------------------------------------------------------------===//
  // Property accessors for dealing with types... Some of these virtual methods
  // are defined in private classes defined in Type.cpp for primitive types.
  //

  /// getTypeID - Return the type id for the type.  This will return one
  /// of the TypeID enum elements defined above.
  ///
  inline TypeID getTypeID() const { return ID; }

  /// getDescription - Return the string representation of the type...
  const std::string &getDescription() const;

  /// isInteger - True if this is an instance of IntegerType.
  ///
  bool isInteger() const { return ID == IntegerTyID; } 

  /// isFloatingPoint - Return true if this is one of the two floating point
  /// types
  bool isFloatingPoint() const { return ID == FloatTyID || ID == DoubleTyID; }

  /// isFPOrFPVector - Return true if this is a FP type or a vector of FP types.
  ///
  bool isFPOrFPVector() const;
  
  /// isAbstract - True if the type is either an Opaque type, or is a derived
  /// type that includes an opaque type somewhere in it.
  ///
  inline bool isAbstract() const { return Abstract; }

  /// canLosslesslyBitCastTo - Return true if this type could be converted 
  /// with a lossless BitCast to type 'Ty'. For example, uint to int. BitCasts 
  /// are valid for types of the same size only where no re-interpretation of 
  /// the bits is done.
  /// @brief Determine if this type could be losslessly bitcast to Ty
  bool canLosslesslyBitCastTo(const Type *Ty) const;


  /// Here are some useful little methods to query what type derived types are
  /// Note that all other types can just compare to see if this == Type::xxxTy;
  ///
  inline bool isPrimitiveType() const { return ID <= LastPrimitiveTyID; }
  inline bool isDerivedType()   const { return ID >= FirstDerivedTyID; }

  /// isFirstClassType - Return true if the value is holdable in a register.
  ///
  inline bool isFirstClassType() const {
    return (ID != VoidTyID && ID <= LastPrimitiveTyID) ||
            ID == IntegerTyID || ID == PointerTyID || ID == VectorTyID;
  }

  /// isSized - Return true if it makes sense to take the size of this type.  To
  /// get the actual size for a particular target, it is reasonable to use the
  /// TargetData subsystem to do this.
  ///
  bool isSized() const {
    // If it's a primitive, it is always sized.
    if (ID == IntegerTyID || isFloatingPoint() || ID == PointerTyID)
      return true;
    // If it is not something that can have a size (e.g. a function or label),
    // it doesn't have a size.
    if (ID != StructTyID && ID != ArrayTyID && ID != VectorTyID &&
        ID != PackedStructTyID)
      return false;
    // If it is something that can have a size and it's concrete, it definitely
    // has a size, otherwise we have to try harder to decide.
    return !isAbstract() || isSizedDerivedType();
  }

  /// getPrimitiveSize - Return the basic size of this type if it is a primitive
  /// type.  These are fixed by LLVM and are not target dependent.  This will
  /// return zero if the type does not have a size or is not a primitive type.
  ///
  unsigned getPrimitiveSizeInBits() const;

  /// getForwaredType - Return the type that this type has been resolved to if
  /// it has been resolved to anything.  This is used to implement the
  /// union-find algorithm for type resolution, and shouldn't be used by general
  /// purpose clients.
  const Type *getForwardedType() const {
    if (!ForwardType) return 0;
    return getForwardedTypeInternal();
  }

  /// getVAArgsPromotedType - Return the type an argument of this type
  /// will be promoted to if passed through a variable argument
  /// function.
  const Type *getVAArgsPromotedType() const; 

  //===--------------------------------------------------------------------===//
  // Type Iteration support
  //
  typedef PATypeHandle *subtype_iterator;
  subtype_iterator subtype_begin() const { return ContainedTys; }
  subtype_iterator subtype_end() const { return &ContainedTys[NumContainedTys];}

  /// getContainedType - This method is used to implement the type iterator
  /// (defined a the end of the file).  For derived types, this returns the
  /// types 'contained' in the derived type.
  ///
  const Type *getContainedType(unsigned i) const {
    assert(i < NumContainedTys && "Index out of range!");
    return ContainedTys[i].get();
  }

  /// getNumContainedTypes - Return the number of types in the derived type.
  ///
  unsigned getNumContainedTypes() const { return NumContainedTys; }

  //===--------------------------------------------------------------------===//
  // Static members exported by the Type class itself.  Useful for getting
  // instances of Type.
  //

  /// getPrimitiveType - Return a type based on an identifier.
  static const Type *getPrimitiveType(TypeID IDNumber);

  //===--------------------------------------------------------------------===//
  // These are the builtin types that are always available...
  //
  static const Type *VoidTy, *LabelTy, *FloatTy, *DoubleTy;
  static const IntegerType *Int1Ty, *Int8Ty, *Int16Ty, *Int32Ty, *Int64Ty;

  /// Methods for support type inquiry through isa, cast, and dyn_cast:
  static inline bool classof(const Type *T) { return true; }

  void addRef() const {
    assert(isAbstract() && "Cannot add a reference to a non-abstract type!");
    ++RefCount;
  }

  void dropRef() const {
    assert(isAbstract() && "Cannot drop a reference to a non-abstract type!");
    assert(RefCount && "No objects are currently referencing this object!");

    // If this is the last PATypeHolder using this object, and there are no
    // PATypeHandles using it, the type is dead, delete it now.
    if (--RefCount == 0 && AbstractTypeUsers.empty())
      this->destroy();
  }
  
  /// addAbstractTypeUser - Notify an abstract type that there is a new user of
  /// it.  This function is called primarily by the PATypeHandle class.
  ///
  void addAbstractTypeUser(AbstractTypeUser *U) const {
    assert(isAbstract() && "addAbstractTypeUser: Current type not abstract!");
    AbstractTypeUsers.push_back(U);
  }
  
  /// removeAbstractTypeUser - Notify an abstract type that a user of the class
  /// no longer has a handle to the type.  This function is called primarily by
  /// the PATypeHandle class.  When there are no users of the abstract type, it
  /// is annihilated, because there is no way to get a reference to it ever
  /// again.
  ///
  void removeAbstractTypeUser(AbstractTypeUser *U) const;

private:
  /// isSizedDerivedType - Derived types like structures and arrays are sized
  /// iff all of the members of the type are sized as well.  Since asking for
  /// their size is relatively uncommon, move this operation out of line.
  bool isSizedDerivedType() const;

  virtual void refineAbstractType(const DerivedType *OldTy, const Type *NewTy);
  virtual void typeBecameConcrete(const DerivedType *AbsTy);

protected:
  // PromoteAbstractToConcrete - This is an internal method used to calculate
  // change "Abstract" from true to false when types are refined.
  void PromoteAbstractToConcrete();
  friend class TypeMapBase;
};

//===----------------------------------------------------------------------===//
// Define some inline methods for the AbstractTypeUser.h:PATypeHandle class.
// These are defined here because they MUST be inlined, yet are dependent on
// the definition of the Type class.
//
inline void PATypeHandle::addUser() {
  assert(Ty && "Type Handle has a null type!");
  if (Ty->isAbstract())
    Ty->addAbstractTypeUser(User);
}
inline void PATypeHandle::removeUser() {
  if (Ty->isAbstract())
    Ty->removeAbstractTypeUser(User);
}

// Define inline methods for PATypeHolder...

inline void PATypeHolder::addRef() {
  if (Ty->isAbstract())
    Ty->addRef();
}

inline void PATypeHolder::dropRef() {
  if (Ty->isAbstract())
    Ty->dropRef();
}


//===----------------------------------------------------------------------===//
// Provide specializations of GraphTraits to be able to treat a type as a
// graph of sub types...

template <> struct GraphTraits<Type*> {
  typedef Type NodeType;
  typedef Type::subtype_iterator ChildIteratorType;

  static inline NodeType *getEntryNode(Type *T) { return T; }
  static inline ChildIteratorType child_begin(NodeType *N) {
    return N->subtype_begin();
  }
  static inline ChildIteratorType child_end(NodeType *N) {
    return N->subtype_end();
  }
};

template <> struct GraphTraits<const Type*> {
  typedef const Type NodeType;
  typedef Type::subtype_iterator ChildIteratorType;

  static inline NodeType *getEntryNode(const Type *T) { return T; }
  static inline ChildIteratorType child_begin(NodeType *N) {
    return N->subtype_begin();
  }
  static inline ChildIteratorType child_end(NodeType *N) {
    return N->subtype_end();
  }
};

template <> inline bool isa_impl<PointerType, Type>(const Type &Ty) {
  return Ty.getTypeID() == Type::PointerTyID;
}

std::ostream &operator<<(std::ostream &OS, const Type &T);

} // End llvm namespace

#endif