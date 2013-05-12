/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
#define _ALL_SOURCE 1
#endif

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have <vfork.h>.  */
/* #undef HAVE_VFORK_H */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if `sys_siglist' is declared by <signal.h>.  */
/* #undef SYS_SIGLIST_DECLARED */

/* Define vfork as fork if vfork does not work.  */
#define vfork fork

/* Define if extern "C" is needed arround include files */
#define EXTERN_C_BEGIN 
#define EXTERN_C_END 

/* Define if you have char* _sys_siglist[] */
#define SYS_SIGLIST sys_siglist

/* Define if you have libg++ */
#define _S_LIBGXX 1

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the <builtin.h> header file.  */
#define HAVE_BUILTIN_H 1

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1
#include <local.h>
