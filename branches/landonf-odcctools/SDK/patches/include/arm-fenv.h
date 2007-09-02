--- fenv.h	2006-09-11 15:30:09.000000000 -0700
+++ fenv.h	2007-09-01 17:59:06.000000000 -0700
@@ -30,6 +30,8 @@
 #include "architecture/ppc/fenv.h"
 #elif (defined (__i386__) || defined( __x86_64__ ))
 #include "architecture/i386/fenv.h"
+#elif defined(__arm__)
+#include <architecture/arm/fenv.h>
 #else
 #error Unknown architecture
 #endif
