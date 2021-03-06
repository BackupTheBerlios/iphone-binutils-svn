//===- llvm/System/Path.h - Path Operating System Concept -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by Reid Spencer and is distributed under the
// University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the llvm::sys::Path class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SYSTEM_PATH_H
#define LLVM_SYSTEM_PATH_H

#include "llvm/System/TimeValue.h"
#include "llvm/System/IncludeFile.h"
#include <set>
#include <string>
#include <vector>
#include <iosfwd>

namespace llvm {
namespace sys {

  /// This structure provides basic file system information about a file. It
  /// is patterned after the stat(2) Unix operating system call but made
  /// platform independent and eliminates many of the unix-specific fields.
  /// However, to support llvm-ar, the mode, user, and group fields are
  /// retained. These pertain to unix security and may not have a meaningful
  /// value on non-Unix platforms. However, the other fields fields should 
  /// always be applicable on all platforms.  The structure is filled in by 
  /// the PathWithStatus class.
  /// @brief File status structure
  class FileStatus {
  public:
    uint64_t    fileSize;   ///< Size of the file in bytes
    TimeValue   modTime;    ///< Time of file's modification
    uint32_t    mode;       ///< Mode of the file, if applicable
    uint32_t    user;       ///< User ID of owner, if applicable
    uint32_t    group;      ///< Group ID of owner, if applicable
    uint64_t    uniqueID;   ///< A number to uniquely ID this file
    bool        isDir  : 1; ///< True if this is a directory.
    bool        isFile : 1; ///< True if this is a file.

    FileStatus() : fileSize(0), modTime(0,0), mode(0777), user(999),
                   group(999), uniqueID(0), isDir(false), isFile(false) { }
    
    TimeValue getTimestamp() const { return modTime; }
    uint64_t getSize() const { return fileSize; }
    uint32_t getMode() const { return mode; }
    uint32_t getUser() const { return user; }
    uint32_t getGroup() const { return group; }
    uint64_t getUniqueID() const { return uniqueID; }
  };

  /// This class provides an abstraction for the path to a file or directory
  /// in the operating system's filesystem and provides various basic operations
  /// on it.  Note that this class only represents the name of a path to a file
  /// or directory which may or may not be valid for a given machine's file
  /// system. The class is patterned after the java.io.File class with various
  /// extensions and several omissions (not relevant to LLVM).  A Path object
  /// ensures that the path it encapsulates is syntactically valid for the
  /// operating system it is running on but does not ensure correctness for
  /// any particular file system. That is, a syntactically valid path might
  /// specify path components that do not exist in the file system and using
  /// such a Path to act on the file system could produce errors. There is one
  /// invalid Path value which is permitted: the empty path.  The class should
  /// never allow a syntactically invalid non-empty path name to be assigned.
  /// Empty paths are required in order to indicate an error result in some
  /// situations. If the path is empty, the isValid operation will return
  /// false. All operations will fail if isValid is false. Operations that
  /// change the path will either return false if it would cause a syntactically
  /// invalid path name (in which case the Path object is left unchanged) or
  /// throw an std::string exception indicating the error. The methods are
  /// grouped into four basic categories: Path Accessors (provide information
  /// about the path without accessing disk), Disk Accessors (provide
  /// information about the underlying file or directory), Path Mutators
  /// (change the path information, not the disk), and Disk Mutators (change
  /// the disk file/directory referenced by the path). The Disk Mutator methods
  /// all have the word "disk" embedded in their method name to reinforce the
  /// notion that the operation modifies the file system.
  /// @since 1.4
  /// @brief An abstraction for operating system paths.
  class Path {
    /// @name Constructors
    /// @{
    public:
      /// Construct a path to the root directory of the file system. The root
      /// directory is a top level directory above which there are no more
      /// directories. For example, on UNIX, the root directory is /. On Windows
      /// it is C:\. Other operating systems may have different notions of
      /// what the root directory is or none at all. In that case, a consistent
      /// default root directory will be used.
      static Path GetRootDirectory();

      /// Construct a path to a unique temporary directory that is created in
      /// a "standard" place for the operating system. The directory is
      /// guaranteed to be created on exit from this function. If the directory
      /// cannot be created, the function will throw an exception.
      /// @returns an invalid path (empty) on error
      /// @param ErrMsg Optional place for an error message if an error occurs
      /// @brief Constrct a path to an new, unique, existing temporary
      /// directory.
      static Path GetTemporaryDirectory(std::string* ErrMsg = 0);

      /// Construct a vector of sys::Path that contains the "standard" system
      /// library paths suitable for linking into programs. This function *must*
      /// return the value of LLVM_LIB_SEARCH_PATH as the first item in \p Paths
      /// if that environment variable is set and it references a directory.
      /// @brief Construct a path to the system library directory
      static void GetSystemLibraryPaths(std::vector<sys::Path>& Paths);

      /// Construct a vector of sys::Path that contains the "standard" bytecode
      /// library paths suitable for linking into an llvm program. This function
      /// *must* return the value of LLVM_LIB_SEARCH_PATH as well as the value
      /// of LLVM_LIBDIR. It also must provide the System library paths as
      /// returned by GetSystemLibraryPaths.
      /// @see GetSystemLibraryPaths
      /// @brief Construct a list of directories in which bytecode could be
      /// found.
      static void GetBytecodeLibraryPaths(std::vector<sys::Path>& Paths);

      /// Find the path to a library using its short name. Use the system
      /// dependent library paths to locate the library.
      /// @brief Find a library.
      static Path FindLibrary(std::string& short_name);

      /// Construct a path to the default LLVM configuration directory. The
      /// implementation must ensure that this is a well-known (same on many
      /// systems) directory in which llvm configuration files exist. For
      /// example, on Unix, the /etc/llvm directory has been selected.
      /// @brief Construct a path to the default LLVM configuration directory
      static Path GetLLVMDefaultConfigDir();

      /// Construct a path to the LLVM installed configuration directory. The
      /// implementation must ensure that this refers to the "etc" directory of
      /// the LLVM installation. This is the location where configuration files
      /// will be located for a particular installation of LLVM on a machine.
      /// @brief Construct a path to the LLVM installed configuration directory
      static Path GetLLVMConfigDir();

      /// Construct a path to the current user's home directory. The
      /// implementation must use an operating system specific mechanism for
      /// determining the user's home directory. For example, the environment
      /// variable "HOME" could be used on Unix. If a given operating system
      /// does not have the concept of a user's home directory, this static
      /// constructor must provide the same result as GetRootDirectory.
      /// @brief Construct a path to the current user's "home" directory
      static Path GetUserHomeDirectory();

      /// Return the suffix commonly used on file names that contain a shared
      /// object, shared archive, or dynamic link library. Such files are
      /// linked at runtime into a process and their code images are shared
      /// between processes.
      /// @returns The dynamic link library suffix for the current platform.
      /// @brief Return the dynamic link library suffix.
      static std::string GetDLLSuffix();

      /// This is one of the very few ways in which a path can be constructed
      /// with a syntactically invalid name. The only *legal* invalid name is an
      /// empty one. Other invalid names are not permitted. Empty paths are
      /// provided so that they can be used to indicate null or error results in
      /// other lib/System functionality.
      /// @brief Construct an empty (and invalid) path.
      Path() : path() {}
      Path(const Path &that) : path(that.path) {}

      /// This constructor will accept a std::string as a path. No checking is
      /// done on this path to determine if it is valid. To determine validity
      /// of the path, use the isValid method. 
      /// @param p The path to assign.
      /// @brief Construct a Path from a string.
      explicit Path(const std::string& p) : path(p) {}

      /// This constructor will accept a character range as a path.  No checking
      /// is done on this path to determine if it is valid.  To determine
      /// validity of the path, use the isValid method. 
      /// @param p The path to assign.
      /// @brief Construct a Path from a string.
      explicit Path(const char *StrStart, unsigned StrLen)
        : path(StrStart, StrStart+StrLen) {}
      
    /// @}
    /// @name Operators
    /// @{
    public:
      /// Makes a copy of \p that to \p this.
      /// @returns \p this
      /// @brief Assignment Operator
      Path &operator=(const Path &that) {
        path = that.path;
        return *this;
      }

      /// Compares \p this Path with \p that Path for equality.
      /// @returns true if \p this and \p that refer to the same thing.
      /// @brief Equality Operator
      bool operator==(const Path &that) const {
        return 0 == path.compare(that.path);
      }

      /// Compares \p this Path with \p that Path for inequality.
      /// @returns true if \p this and \p that refer to different things.
      /// @brief Inequality Operator
      bool operator!=(const Path &that) const {
        return 0 != path.compare(that.path);
      }

      /// Determines if \p this Path is less than \p that Path. This is required
      /// so that Path objects can be placed into ordered collections (e.g.
      /// std::map). The comparison is done lexicographically as defined by
      /// the std::string::compare method.
      /// @returns true if \p this path is lexicographically less than \p that.
      /// @brief Less Than Operator
      bool operator<(const Path& that) const {
        return 0 > path.compare(that.path);
      }

    /// @}
    /// @name Path Accessors
    /// @{
    public:
      /// This function will use an operating system specific algorithm to
      /// determine if the current value of \p this is a syntactically valid
      /// path name for the operating system. The path name does not need to
      /// exist, validity is simply syntactical. Empty paths are always invalid.
      /// @returns true iff the path name is syntactically legal for the
      /// host operating system.
      /// @brief Determine if a path is syntactically valid or not.
      bool isValid() const;

      /// This function determines if the contents of the path name are empty. 
      /// That is, the path name has a zero length. This does NOT determine if
      /// if the file is empty. To get the length of the file itself, Use the 
      /// PathWithStatus::getFileStatus() method and then the getSize() method 
      /// on the returned FileStatus object.
      /// @returns true iff the path is empty.
      /// @brief Determines if the path name is empty (invalid).
      bool isEmpty() const { return path.empty(); }

      /// This function returns the current contents of the path as a
      /// std::string. This allows the underlying path string to be manipulated.
      /// @returns std::string containing the path name.
      /// @brief Returns the path as a std::string.
      const std::string &toString() const { return path; }

      /// This function returns the last component of the path name. The last
      /// component is the file or directory name occuring after the last
      /// directory separator. If no directory separator is present, the entire
      /// path name is returned (i.e. same as toString).
      /// @returns std::string containing the last component of the path name.
      /// @brief Returns the last component of the path name.
      std::string getLast() const;

      /// This function strips off the path and suffix of the file or directory
      /// name and returns just the basename. For example /a/foo.bar would cause
      /// this function to return "foo".
      /// @returns std::string containing the basename of the path
      /// @brief Get the base name of the path
      std::string getBasename() const;

      /// Obtain a 'C' string for the path name.
      /// @returns a 'C' string containing the path name.
      /// @brief Returns the path as a C string.
      const char *c_str() const { return path.c_str(); }

    /// @}
    /// @name Disk Accessors
    /// @{
    public:
      /// This function determines if the path name in this object references
      /// the root (top level directory) of the file system. The details of what
      /// is considered the "root" may vary from system to system so this method
      /// will do the necessary checking.
      /// @returns true iff the path name references the root directory.
      /// @brief Determines if the path references the root directory.
      bool isRootDirectory() const;

      /// This function determines if the path name is absolute, as opposed to
      /// relative. 
      /// @breif Determine if the path is absolute.
      bool isAbsolute() const;

      /// This function opens the file associated with the path name provided by
      /// the Path object and reads its magic number. If the magic number at the
      /// start of the file matches \p magic, true is returned. In all other
      /// cases (file not found, file not accessible, etc.) it returns false.
      /// @returns true if the magic number of the file matches \p magic.
      /// @brief Determine if file has a specific magic number
      bool hasMagicNumber(const std::string& magic) const;

      /// This function retrieves the first \p len bytes of the file associated
      /// with \p this. These bytes are returned as the "magic number" in the
      /// \p Magic parameter.
      /// @returns true if the Path is a file and the magic number is retrieved,
      /// false otherwise.
      /// @brief Get the file's magic number.
      bool getMagicNumber(std::string& Magic, unsigned len) const;

      /// This function determines if the path name in the object references an
      /// archive file by looking at its magic number.
      /// @returns true if the file starts with the magic number for an archive
      /// file.
      /// @brief Determine if the path references an archive file.
      bool isArchive() const;

      /// This function determines if the path name in the object references an
      /// LLVM Bytecode file by looking at its magic number.
      /// @returns true if the file starts with the magic number for LLVM
      /// bytecode files.
      /// @brief Determine if the path references a bytecode file.
      bool isBytecodeFile() const;

      /// This function determines if the path name in the object references an
      /// LLVM Bitcode file by looking at its magic number.
      /// @returns true if the file starts with the magic number for LLVM
      /// bitcode files.
      /// @brief Determine if the path references a bitcode file.
      bool isBitcodeFile() const;
      
      /// This function determines if the path name in the object references a
      /// native Dynamic Library (shared library, shared object) by looking at
      /// the file's magic number. The Path object must reference a file, not a
      /// directory.
      /// @return strue if the file starts with the magid number for a native
      /// shared library.
      /// @brief Determine if the path reference a dynamic library.
      bool isDynamicLibrary() const;

      /// This function determines if the path name references an existing file
      /// or directory in the file system.
      /// @returns true if the pathname references an existing file or
      /// directory.
      /// @brief Determines if the path is a file or directory in
      /// the file system.
      bool exists() const;

      /// This function determines if the path name references a readable file
      /// or directory in the file system. This function checks for
      /// the existence and readability (by the current program) of the file
      /// or directory.
      /// @returns true if the pathname references a readable file.
      /// @brief Determines if the path is a readable file or directory
      /// in the file system.
      bool canRead() const;

      /// This function determines if the path name references a writable file
      /// or directory in the file system. This function checks for the
      /// existence and writability (by the current program) of the file or
      /// directory.
      /// @returns true if the pathname references a writable file.
      /// @brief Determines if the path is a writable file or directory
      /// in the file system.
      bool canWrite() const;

      /// This function determines if the path name references an executable
      /// file in the file system. This function checks for the existence and
      /// executability (by the current program) of the file.
      /// @returns true if the pathname references an executable file.
      /// @brief Determines if the path is an executable file in the file
      /// system.
      bool canExecute() const;

      /// This function builds a list of paths that are the names of the
      /// files and directories in a directory.
      /// @returns true if an error occurs, true otherwise
      /// @brief Build a list of directory's contents.
      bool getDirectoryContents(
        std::set<Path> &paths, ///< The resulting list of file & directory names
        std::string* ErrMsg    ///< Optional place to return an error message.
      ) const;

    /// @}
    /// @name Path Mutators
    /// @{
    public:
      /// The path name is cleared and becomes empty. This is an invalid
      /// path name but is the *only* invalid path name. This is provided
      /// so that path objects can be used to indicate the lack of a
      /// valid path being found.
      /// @brief Make the path empty.
      void clear() { path.clear(); }

      /// This method sets the Path object to \p unverified_path. This can fail
      /// if the \p unverified_path does not pass the syntactic checks of the
      /// isValid() method. If verification fails, the Path object remains
      /// unchanged and false is returned. Otherwise true is returned and the
      /// Path object takes on the path value of \p unverified_path
      /// @returns true if the path was set, false otherwise.
      /// @param unverified_path The path to be set in Path object.
      /// @brief Set a full path from a std::string
      bool set(const std::string& unverified_path);

      /// One path component is removed from the Path. If only one component is
      /// present in the path, the Path object becomes empty. If the Path object
      /// is empty, no change is made.
      /// @returns false if the path component could not be removed.
      /// @brief Removes the last directory component of the Path.
      bool eraseComponent();

      /// The \p component is added to the end of the Path if it is a legal
      /// name for the operating system. A directory separator will be added if
      /// needed.
      /// @returns false if the path component could not be added.
      /// @brief Appends one path component to the Path.
      bool appendComponent( const std::string& component );

      /// A period and the \p suffix are appended to the end of the pathname.
      /// The precondition for this function is that the Path reference a file
      /// name (i.e. isFile() returns true). If the Path is not a file, no
      /// action is taken and the function returns false. If the path would
      /// become invalid for the host operating system, false is returned.
      /// @returns false if the suffix could not be added, true if it was.
      /// @brief Adds a period and the \p suffix to the end of the pathname.
      bool appendSuffix(const std::string& suffix);

      /// The suffix of the filename is erased. The suffix begins with and
      /// includes the last . character in the filename after the last directory
      /// separator and extends until the end of the name. If no . character is
      /// after the last directory separator, then the file name is left
      /// unchanged (i.e. it was already without a suffix) but the function
      /// returns false.
      /// @returns false if there was no suffix to remove, true otherwise.
      /// @brief Remove the suffix from a path name.
      bool eraseSuffix();

      /// The current Path name is made unique in the file system. Upon return,
      /// the Path will have been changed to make a unique file in the file
      /// system or it will not have been changed if the current path name is
      /// already unique.
      /// @throws std::string if an unrecoverable error occurs.
      /// @brief Make the current path name unique in the file system.
      bool makeUnique( bool reuse_current /*= true*/, std::string* ErrMsg );

    /// @}
    /// @name Disk Mutators
    /// @{
    public:
      /// This method attempts to make the file referenced by the Path object
      /// available for reading so that the canRead() method will return true.
      /// @brief Make the file readable;
      bool makeReadableOnDisk(std::string* ErrMsg = 0);

      /// This method attempts to make the file referenced by the Path object
      /// available for writing so that the canWrite() method will return true.
      /// @brief Make the file writable;
      bool makeWriteableOnDisk(std::string* ErrMsg = 0);

      /// This method attempts to make the file referenced by the Path object
      /// available for execution so that the canExecute() method will return
      /// true.
      /// @brief Make the file readable;
      bool makeExecutableOnDisk(std::string* ErrMsg = 0);

      /// This method allows the last modified time stamp and permission bits
      /// to be set on the disk object referenced by the Path.
      /// @throws std::string if an error occurs.
      /// @returns true on error.
      /// @brief Set the status information.
      bool setStatusInfoOnDisk(const FileStatus &SI,
                               std::string *ErrStr = 0) const;

      /// This method attempts to create a directory in the file system with the
      /// same name as the Path object. The \p create_parents parameter controls
      /// whether intermediate directories are created or not. if \p
      /// create_parents is true, then an attempt will be made to create all
      /// intermediate directories, as needed. If \p create_parents is false,
      /// then only the final directory component of the Path name will be
      /// created. The created directory will have no entries.
      /// @returns true if the directory could not be created, false otherwise
      /// @brief Create the directory this Path refers to.
      bool createDirectoryOnDisk( 
        bool create_parents = false, ///<  Determines whether non-existent 
           ///< directory components other than the last one (the "parents") 
           ///< are created or not.
        std::string* ErrMsg = 0 ///< Optional place to put error messages.
      );

      /// This method attempts to create a file in the file system with the same
      /// name as the Path object. The intermediate directories must all exist
      /// at the time this method is called. Use createDirectoriesOnDisk to
      /// accomplish that. The created file will be empty upon return from this
      /// function.
      /// @returns true if the file could not be created, false otherwise.
      /// @brief Create the file this Path refers to.
      bool createFileOnDisk(
        std::string* ErrMsg = 0 ///< Optional place to put error messages.
      );

      /// This is like createFile except that it creates a temporary file. A
      /// unique temporary file name is generated based on the contents of
      /// \p this before the call. The new name is assigned to \p this and the
      /// file is created.  Note that this will both change the Path object
      /// *and* create the corresponding file. This function will ensure that
      /// the newly generated temporary file name is unique in the file system.
      /// @returns true if the file couldn't be created, false otherwise. 
      /// @brief Create a unique temporary file
      bool createTemporaryFileOnDisk(
        bool reuse_current = false, ///< When set to true, this parameter 
          ///< indicates that if the current file name does not exist then 
          ///< it will be used without modification.
        std::string* ErrMsg = 0 ///< Optional place to put error messages
      );

      /// This method renames the file referenced by \p this as \p newName. The
      /// file referenced by \p this must exist. The file referenced by
      /// \p newName does not need to exist.
      /// @returns true on error, false otherwise
      /// @brief Rename one file as another.
      bool renamePathOnDisk(const Path& newName, std::string* ErrMsg);

      /// This method attempts to destroy the file or directory named by the
      /// last component of the Path. If the Path refers to a directory and the
      /// \p destroy_contents is false, an attempt will be made to remove just
      /// the directory (the final Path component). If \p destroy_contents is
      /// true, an attempt will be made to remove the entire contents of the
      /// directory, recursively. If the Path refers to a file, the
      /// \p destroy_contents parameter is ignored.
      /// @param destroy_contents Indicates whether the contents of a destroyed
      /// directory should also be destroyed (recursively).
      /// @returns false if the file/directory was destroyed, true on error.
      /// @brief Removes the file or directory from the filesystem.
      bool eraseFromDisk(bool destroy_contents = false,
                         std::string *Err = 0) const;
    /// @}
    /// @name Data
    /// @{
    protected:
      mutable std::string path;   ///< Storage for the path name.

    /// @}
  };

  /// This class is identical to Path class except it allows you to obtain the
  /// file status of the Path as well. The reason for the distinction is one of
  /// efficiency. First, the file status requires additional space and the space
  /// is incorporated directly into PathWithStatus without an additional malloc.
  /// Second, obtaining status information is an expensive operation on most
  /// operating systems so we want to be careful and explicity about where we
  /// allow this operation in LLVM.
  /// @brief Path with file status class.
  class PathWithStatus : public Path {
    /// @name Constructors
    /// @{
    public: 
      /// @brief Default constructor
      PathWithStatus() : Path(), status(), fsIsValid(false) {}

      /// @brief Copy constructor
      PathWithStatus(const PathWithStatus &that) 
        : Path(static_cast<const Path&>(that)), status(that.status), 
           fsIsValid(that.fsIsValid) {}

      /// This constructor allows construction from a Path object
      /// @brief Path constructor
      PathWithStatus(const Path &other) 
        : Path(other), status(), fsIsValid(false) {}

      /// This constructor will accept a std::string as a path. No checking is
      /// done on this path to determine if it is valid. To determine validity
      /// of the path, use the isValid method. 
      /// @param p The path to assign.
      /// @brief Construct a Path from a string.
      explicit PathWithStatus(const std::string& p) 
        : Path(p), status(), fsIsValid(false) {}

      /// This constructor will accept a character range as a path.  No checking
      /// is done on this path to determine if it is valid.  To determine
      /// validity of the path, use the isValid method. 
      /// @param p The path to assign.
      /// @brief Construct a Path from a string.
      explicit PathWithStatus(const char *StrStart, unsigned StrLen)
        : Path(StrStart, StrLen), status(), fsIsValid(false) {}

      /// Makes a copy of \p that to \p this.
      /// @returns \p this
      /// @brief Assignment Operator
      PathWithStatus &operator=(const PathWithStatus &that) {
        static_cast<Path&>(*this) = static_cast<const Path&>(that);
        status = that.status;
        fsIsValid = that.fsIsValid;
        return *this;
      }

      /// Makes a copy of \p that to \p this.
      /// @returns \p this
      /// @brief Assignment Operator
      PathWithStatus &operator=(const Path &that) {
        static_cast<Path&>(*this) = static_cast<const Path&>(that);
        fsIsValid = false;
        return *this;
      }

    /// @}
    /// @name Methods
    /// @{
    public:
      /// This function returns status information about the file. The type of
      /// path (file or directory) is updated to reflect the actual contents
      /// of the file system.
      /// @returns 0 on failure, with Error explaining why (if non-zero)
      /// @returns a pointer to a FileStatus structure on success.
      /// @brief Get file status.
      const FileStatus *getFileStatus(
        bool forceUpdate = false, ///< Force an update from the file system
        std::string *Error = 0    ///< Optional place to return an error msg.
      ) const;

    /// @}
    /// @name Data
    /// @{
    private:
      mutable FileStatus status; ///< Status information.
      mutable bool fsIsValid;    ///< Whether we've obtained it or not

    /// @}
  };

  /// This enumeration delineates the kinds of files that LLVM knows about.
  enum LLVMFileType {
    Unknown_FileType = 0,              ///< Unrecognized file
    Bytecode_FileType,                 ///< Uncompressed bytecode file
    Bitcode_FileType,                  ///< Bitcode file
    CompressedBytecode_FileType,       ///< Compressed bytecode file
    Archive_FileType,                  ///< ar style archive file
    ELF_Relocatable_FileType,          ///< ELF Relocatable object file
    ELF_Executable_FileType,           ///< ELF Executable image
    ELF_SharedObject_FileType,         ///< ELF dynamically linked shared lib
    ELF_Core_FileType,                 ///< ELF core image
    Mach_O_Object_FileType,            ///< Mach-O Object file
    Mach_O_Executable_FileType,        ///< Mach-O Executable
    Mach_O_FixedVirtualMemorySharedLib_FileType, ///< Mach-O Shared Lib, FVM
    Mach_O_Core_FileType,              ///< Mach-O Core File
    Mach_O_PreloadExectuable_FileType, ///< Mach-O Preloaded Executable
    Mach_O_DynamicallyLinkedSharedLib_FileType, ///< Mach-O dynlinked shared lib
    Mach_O_DynamicLinker_FileType,     ///< The Mach-O dynamic linker
    Mach_O_Bundle_FileType,            ///< Mach-O Bundle file
    Mach_O_DynamicallyLinkedSharedLibStub_FileType, ///< Mach-O Shared lib stub
    COFF_FileType                      ///< COFF object file or lib
  };

  /// This utility function allows any memory block to be examined in order
  /// to determine its file type.
  LLVMFileType IdentifyFileType(const char*magic, unsigned length);

  /// This function can be used to copy the file specified by Src to the
  /// file specified by Dest. If an error occurs, Dest is removed.
  /// @returns true if an error occurs, false otherwise
  /// @brief Copy one file to another.
  bool CopyFile(const Path& Dest, const Path& Src, std::string* ErrMsg);
}

std::ostream& operator<<(std::ostream& strm, const sys::Path& aPath);
inline std::ostream& operator<<(std::ostream& strm, 
                                const sys::PathWithStatus& aPath) {
  strm << static_cast<const sys::Path&>(aPath);
  return strm;
}

}

FORCE_DEFINING_FILE_TO_BE_LINKED(SystemPath)
#endif
