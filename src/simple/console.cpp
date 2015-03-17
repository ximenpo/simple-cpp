#include "simple/console.h"

//
//	代码实现
//
#if	defined(_WIN32)
#	include	"console_win32.inc"
#else
#	include	"console_unix.inc"
#endif
