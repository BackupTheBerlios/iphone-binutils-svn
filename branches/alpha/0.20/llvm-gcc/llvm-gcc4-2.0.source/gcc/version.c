#include "version.h"

/* This is the string reported as the version number by all components
   of the compiler.  If you distribute a modified version of GCC,
   please modify this string to indicate that, e.g. by putting your
   organization's name in parentheses at the end of the string.  */

/* APPLE LOCAL begin Apple version */
/* When updating this string:
   - For each internal build, increment the build number.
   - When merging from the FSF, delete any (experimental) or (prerelease).
     Apple doesn't mark its GCC versions as 'prerelease', because a released
     compiler will be identical to the last prerelease compiler and it
     makes no sense to mark released compilers as 'prerelease'.
   - There are other scripts that search for first word of the string
     to get version number string. Do not use new line.
*/

const char version_string[] = "4.0.1 "
#ifdef ENABLE_LLVM
                              "LLVM "
#endif
                              "(Apple Computer, Inc. build "
#ifdef LLVM_VERSION_INFO
                              LLVM_VERSION_INFO
#else
                              "5449"
#endif
                              ")";
/* APPLE LOCAL end Apple version */

/* This is the location of the online document giving instructions for
   reporting bugs.  If you distribute a modified version of GCC,
   please change this to refer to a document giving instructions for
   reporting bugs to you, not us.  (You are of course welcome to
   forward us bugs reported to you, if you determine that they are
   not bugs in your modifications.)  */

/* APPLE LOCAL begin Apple bug-report */
/* APPLE LOCAL LLVM bug URL */
const char bug_report_url[] = "<URL:http://llvm.org/bugs>";
/* APPLE LOCAL end Apple bug-report */
