//===- lib/Linker/LinkItems.cpp - Link LLVM objects and libraries ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Reid Spencer and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains routines to handle linking together LLVM bytecode files,
// and to handle annoying things like static libraries.
//
//===----------------------------------------------------------------------===//

#include "llvm/Linker.h"
#include "llvm/Module.h"

using namespace llvm;

// LinkItems - This function is the main entry point into linking. It takes a
// list of LinkItem which indicates the order the files should be linked and
// how each file should be treated (plain file or with library search). The
// function only links bytecode and produces a result list of items that are
// native objects. 
bool
Linker::LinkInItems(const ItemList& Items, ItemList& NativeItems) {
  // Clear the NativeItems just in case
  NativeItems.clear();

  // For each linkage item ...
  for (ItemList::const_iterator I = Items.begin(), E = Items.end();
       I != E; ++I) {
    if (I->second) {
      // Link in the library suggested.
      bool is_native = false;
      if (LinkInLibrary(I->first, is_native))
        return true;
      if (is_native)
        NativeItems.push_back(*I);
    } else {
      // Link in the file suggested
      bool is_native = false;
      if (LinkInFile(sys::Path(I->first), is_native))
        return true;
      if (is_native)
        NativeItems.push_back(*I);
    }
  }

  // At this point we have processed all the link items provided to us. Since
  // we have an aggregated module at this point, the dependent libraries in
  // that module should also be aggregated with duplicates eliminated. This is
  // now the time to process the dependent libraries to resolve any remaining
  // symbols.
  bool is_native;
  for (Module::lib_iterator I = Composite->lib_begin(),
         E = Composite->lib_end(); I != E; ++I) {
    if(LinkInLibrary(*I, is_native))
      return true;
    if (is_native)
      NativeItems.push_back(std::make_pair(*I, true));
  }

  return false;
}


/// LinkInLibrary - links one library into the HeadModule.
///
bool Linker::LinkInLibrary(const std::string& Lib, bool& is_native) {
  is_native = false;
  // Determine where this library lives.
  sys::Path Pathname = FindLib(Lib);
  if (Pathname.isEmpty())
    return warning("Cannot find library '" + Lib + "'");

  // If its an archive, try to link it in
  std::string Magic;
  Pathname.getMagicNumber(Magic, 64);
  switch (sys::IdentifyFileType(Magic.c_str(), 64)) {
    default: assert(0 && "Bad file type identification");
    case sys::Unknown_FileType:
      return warning("Supposed library '" + Lib + "' isn't a library.");

    case sys::Bytecode_FileType:
    case sys::Bitcode_FileType:
    case sys::CompressedBytecode_FileType:
      // LLVM ".so" file.
      if (LinkInFile(Pathname, is_native))
        return error("Cannot link file '" + Pathname.toString() + "'");
      break;

    case sys::Archive_FileType:
      if (LinkInArchive(Pathname, is_native))
        return error("Cannot link archive '" + Pathname.toString() + "'");
      break;

    case sys::ELF_Relocatable_FileType:
    case sys::ELF_SharedObject_FileType:
    case sys::Mach_O_Object_FileType:
    case sys::Mach_O_FixedVirtualMemorySharedLib_FileType:
    case sys::Mach_O_DynamicallyLinkedSharedLib_FileType:
    case sys::Mach_O_DynamicallyLinkedSharedLibStub_FileType:
    case sys::COFF_FileType:
      is_native = true;
      break;
  }
  return false;
}

/// LinkLibraries - takes the specified library files and links them into the
/// main bytecode object file.
///
/// Inputs:
///  Libraries  - The list of libraries to link into the module.
///
/// Return value:
///  FALSE - No error.
///  TRUE  - Error.
///
bool Linker::LinkInLibraries(const std::vector<std::string> &Libraries) {

  // Process the set of libraries we've been provided.
  bool is_native = false;
  for (unsigned i = 0; i < Libraries.size(); ++i)
    if (LinkInLibrary(Libraries[i], is_native))
      return true;

  // At this point we have processed all the libraries provided to us. Since
  // we have an aggregated module at this point, the dependent libraries in
  // that module should also be aggregated with duplicates eliminated. This is
  // now the time to process the dependent libraries to resolve any remaining
  // symbols.
  const Module::LibraryListType& DepLibs = Composite->getLibraries();
  for (Module::LibraryListType::const_iterator I = DepLibs.begin(),
         E = DepLibs.end(); I != E; ++I)
    if (LinkInLibrary(*I, is_native))
      return true;

  return false;
}

/// LinkInFile - opens a bytecode file and links in all objects which
/// provide symbols that are currently undefined.
///
/// Inputs:
///  File - The pathname of the bytecode file.
///
/// Outputs:
///  ErrorMessage - A C++ string detailing what error occurred, if any.
///
/// Return Value:
///  TRUE  - An error occurred.
///  FALSE - No errors.
///
bool Linker::LinkInFile(const sys::Path &File, bool &is_native) {
  is_native = false;
  // Make sure we can at least read the file
  if (!File.canRead())
    return error("Cannot find linker input '" + File.toString() + "'");

  // If its an archive, try to link it in
  std::string Magic;
  File.getMagicNumber(Magic, 64);
  switch (sys::IdentifyFileType(Magic.c_str(), 64)) {
    default: assert(0 && "Bad file type identification");
    case sys::Unknown_FileType:
      return warning("Supposed object file '" + File.toString() + 
                     "' not recognized as such");

    case sys::Archive_FileType:
      // A user may specify an ar archive without -l, perhaps because it
      // is not installed as a library. Detect that and link the archive.
      verbose("Linking archive file '" + File.toString() + "'");
      if (LinkInArchive(File, is_native))
        return error("Cannot link archive '" + File.toString() + "'");
      break;

    case sys::Bitcode_FileType:
    case sys::Bytecode_FileType:
    case sys::CompressedBytecode_FileType: {
      verbose("Linking bytecode file '" + File.toString() + "'");
      std::auto_ptr<Module> M(LoadObject(File));
      if (M.get() == 0)
        return error("Cannot load file '" + File.toString() + "'" + Error);
      if (LinkInModule(M.get()))
        return error("Cannot link file '" + File.toString() + "'" + Error);

      verbose("Linked in file '" + File.toString() + "'");
      break;
    }

    case sys::ELF_Relocatable_FileType:
    case sys::ELF_SharedObject_FileType:
    case sys::Mach_O_Object_FileType:
    case sys::Mach_O_FixedVirtualMemorySharedLib_FileType:
    case sys::Mach_O_DynamicallyLinkedSharedLib_FileType:
    case sys::Mach_O_DynamicallyLinkedSharedLibStub_FileType:
    case sys::COFF_FileType:
      is_native = true;
      break;
  }
  return false;
}

/// LinkFiles - takes a module and a list of files and links them all together.
/// It locates the file either in the current directory, as its absolute
/// or relative pathname, or as a file somewhere in LLVM_LIB_SEARCH_PATH.
///
/// Inputs:
///  Files      - A vector of sys::Path indicating the LLVM bytecode filenames
///               to be linked.  The names can refer to a mixture of pure LLVM
///               bytecode files and archive (ar) formatted files.
///
/// Return value:
///  FALSE - No errors.
///  TRUE  - Some error occurred.
///
bool Linker::LinkInFiles(const std::vector<sys::Path> &Files) {
  bool is_native;
  for (unsigned i = 0; i < Files.size(); ++i)
    if (LinkInFile(Files[i], is_native))
      return true;
  return false;
}
