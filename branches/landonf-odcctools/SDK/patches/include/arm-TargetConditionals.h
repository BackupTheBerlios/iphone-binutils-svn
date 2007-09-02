--- TargetConditionals.h	2006-09-11 19:13:49.000000000 -0700
+++ TargetConditionals.h	2007-09-01 11:55:43.000000000 -0700
@@ -67,6 +67,7 @@
         #define TARGET_CPU_MIPS         0
         #define TARGET_CPU_SPARC        0   
         #define TARGET_CPU_ALPHA        0
+        #define TARGET_CPU_ARM          0 
         #define TARGET_RT_LITTLE_ENDIAN 0
         #define TARGET_RT_BIG_ENDIAN    1
         #define TARGET_RT_64_BIT        0
@@ -86,6 +87,7 @@
         #define TARGET_CPU_MIPS         0
         #define TARGET_CPU_SPARC        0   
         #define TARGET_CPU_ALPHA        0
+        #define TARGET_CPU_ARM          0 
         #define TARGET_RT_LITTLE_ENDIAN 0
         #define TARGET_RT_BIG_ENDIAN    1
         #define TARGET_RT_64_BIT        1
@@ -100,6 +102,7 @@
         #define TARGET_CPU_MIPS         0
         #define TARGET_CPU_SPARC        0
         #define TARGET_CPU_ALPHA        0
+        #define TARGET_CPU_ARM          0 
         #define TARGET_RT_MAC_CFM       0
         #define TARGET_RT_MAC_MACHO     1
         #define TARGET_RT_LITTLE_ENDIAN 1
@@ -114,11 +117,27 @@
         #define TARGET_CPU_MIPS         0
         #define TARGET_CPU_SPARC        0
         #define TARGET_CPU_ALPHA        0
+        #define TARGET_CPU_ARM          0 
         #define TARGET_RT_MAC_CFM       0
         #define TARGET_RT_MAC_MACHO     1
         #define TARGET_RT_LITTLE_ENDIAN 1
         #define TARGET_RT_BIG_ENDIAN    0
         #define TARGET_RT_64_BIT        1
+     #elif defined(__arm__)
+        #define TARGET_CPU_PPC          0
+        #define TARGET_CPU_PPC64        0
+        #define TARGET_CPU_68K          0
+        #define TARGET_CPU_X86          0
+        #define TARGET_CPU_X86_64       0
+        #define TARGET_CPU_MIPS         0
+        #define TARGET_CPU_SPARC        0
+        #define TARGET_CPU_ALPHA        0
+        #define TARGET_CPU_ARM          1
+        #define TARGET_RT_MAC_CFM       0
+        #define TARGET_RT_MAC_MACHO     1
+        #define TARGET_RT_LITTLE_ENDIAN 1
+        #define TARGET_RT_BIG_ENDIAN    0
+        #define TARGET_RT_64_BIT        0
     #else
         #error unrecognized GNU C compiler
     #endif
@@ -175,6 +194,7 @@
         #define TARGET_CPU_MIPS     0
         #define TARGET_CPU_SPARC    0
         #define TARGET_CPU_ALPHA    0
+        #define TARGET_CPU_ARM      0 
     #elif defined(TARGET_CPU_PPC64) && TARGET_CPU_PPC64
         #define TARGET_CPU_PPC      0
         #define TARGET_CPU_68K      0
@@ -183,6 +203,7 @@
         #define TARGET_CPU_MIPS     0
         #define TARGET_CPU_SPARC    0
         #define TARGET_CPU_ALPHA    0
+        #define TARGET_CPU_ARM      0 
     #elif defined(TARGET_CPU_X86) && TARGET_CPU_X86
         #define TARGET_CPU_PPC      0
         #define TARGET_CPU_PPC64    0
@@ -191,6 +212,7 @@
         #define TARGET_CPU_MIPS     0
         #define TARGET_CPU_SPARC    0
         #define TARGET_CPU_ALPHA    0
+        #define TARGET_CPU_ARM      0 
     #elif defined(TARGET_CPU_X86_64) && TARGET_CPU_X86_64
         #define TARGET_CPU_PPC      0
         #define TARGET_CPU_PPC64    0
@@ -199,6 +221,16 @@
         #define TARGET_CPU_MIPS     0
         #define TARGET_CPU_SPARC    0
         #define TARGET_CPU_ALPHA    0
+        #define TARGET_CPU_ARM      0 
+    #elif defined(TARGET_CPU_ARM) && TARGET_CPU_ARM
+        #define TARGET_CPU_PPC      0
+        #define TARGET_CPU_PPC64    0
+        #define TARGET_CPU_X86      0
+        #define TARGET_CPU_X86_64   0 
+        #define TARGET_CPU_68K      0
+        #define TARGET_CPU_MIPS     0
+        #define TARGET_CPU_SPARC    0
+        #define TARGET_CPU_ALPHA    0
     #else
         /*
             NOTE:   If your compiler errors out here then support for your compiler 
@@ -217,6 +249,7 @@
         */
         #error TargetConditionals.h: unknown compiler (see comment above)
         #define TARGET_CPU_PPC    0
+        #define TARGET_CPU_ARM    0
         #define TARGET_CPU_68K    0
         #define TARGET_CPU_X86    0
         #define TARGET_CPU_MIPS   0
