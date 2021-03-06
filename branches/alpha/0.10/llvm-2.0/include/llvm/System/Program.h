//===- llvm/System/Program.h ------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Reid Spencer and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the llvm::sys::Program class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SYSTEM_PROGRAM_H
#define LLVM_SYSTEM_PROGRAM_H

#include "llvm/System/Path.h"
#include "llvm/System/IncludeFile.h"
#include <vector>

namespace llvm {
namespace sys {

  /// This class provides an abstraction for programs that are executable by the
  /// operating system. It provides a platform generic way to find executable
  /// programs from the path and to execute them in various ways. The sys::Path
  /// class is used to specify the location of the Program.
  /// @since 1.4
  /// @brief An abstraction for finding and executing programs.
  class Program {
    /// @name Methods
    /// @{
    public:
      /// This static constructor (factory) will attempt to locate a program in
      /// the operating system's file system using some pre-determined set of
      /// locations to search (e.g. the PATH on Unix).
      /// @returns A Path object initialized to the path of the program or a
      /// Path object that is empty (invalid) if the program could not be found.
      /// @throws nothing
      /// @brief Construct a Program by finding it by name.
      static Path FindProgramByName(const std::string& name);

      /// This function executes the program using the \p arguments provided and
      /// waits for the program to exit. This function will block the current
      /// program until the invoked program exits. The invoked program will
      /// inherit the stdin, stdout, and stderr file descriptors, the
      /// environment and other configuration settings of the invoking program.
      /// If Path::executable() does not return true when this function is
      /// called then a std::string is thrown.
      /// @returns an integer result code indicating the status of the program.
      /// A zero or positive value indicates the result code of the program. A
      /// negative value is the signal number on which it terminated. 
      /// @see FindProgrambyName
      /// @brief Executes the program with the given set of \p args.
      static int ExecuteAndWait(
        const Path& path,  ///< sys::Path object providing the path of the 
          ///< program to be executed. It is presumed this is the result of 
          ///< the FindProgramByName method.
        const char** args, ///< A vector of strings that are passed to the
          ///< program.  The first element should be the name of the program.
          ///< The list *must* be terminated by a null char* entry.
        const char ** env = 0, ///< An optional vector of strings to use for
          ///< the program's environment. If not provided, the current program's
          ///< environment will be used.
        const sys::Path** redirects = 0, ///< An optional array of pointers to
          ///< Paths. If the array is null, no redirection is done. The array
          ///< should have a size of at least three. If the pointer in the array
          ///< are not null, then the inferior process's stdin(0), stdout(1),
          ///< and stderr(2) will be redirected to the corresponding Paths.
        unsigned secondsToWait = 0, ///< If non-zero, this specifies the amount
          ///< of time to wait for the child process to exit. If the time
          ///< expires, the child is killed and this call returns. If zero,
          ///< this function will wait until the child finishes or forever if
          ///< it doesn't.
        unsigned memoryLimit = 0, ///< If non-zero, this specifies max. amount
          ///< of memory can be allocated by process. If memory usage will be
          ///< higher limit, the child is killed and this call returns. If zero -
          ///< no memory limit.
        std::string* ErrMsg = 0 ///< If non-zero, provides a pointer to a string
          ///< instance in which error messages will be returned. If the string 
          ///< is non-empty upon return an error occurred while invoking the
          ///< program.
      );
      // These methods change the specified standard stream (stdin or stdout) to
      // binary mode. They return true if an error occurred
      static bool ChangeStdinToBinary();
      static bool ChangeStdoutToBinary();
    /// @}
  };
}
}

FORCE_DEFINING_FILE_TO_BE_LINKED(SystemProgram)

#endif
