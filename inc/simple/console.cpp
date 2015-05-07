#include "console.h"

//
//	代码实现
//
#if	defined(_WIN32)
#	include	"_impls/console_win32.inc"
#else
#	include	"_impls/console_unix.inc"
#endif
