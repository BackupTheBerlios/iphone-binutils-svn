--- math.h	2006-09-11 15:30:09.000000000 -0700
+++ math.h	2007-09-01 18:02:59.000000000 -0700
@@ -26,6 +26,8 @@
 #include "architecture/ppc/math.h"
 #elif (defined (__i386__) || defined( __x86_64__ ))
 #include "architecture/i386/math.h"
+#elif defined(__arm__)
+#include "architecture/arm/math.h"
 #else
 #error Unknown architecture
 #endif
