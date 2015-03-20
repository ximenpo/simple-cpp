
#if			defined(JW_GLOBAL_CONTROL_STRUCT)
#	undef	JW_GLOBAL_CONTROL_STRUCT
#endif

#define		jwErrorToString		jwErrorToString_no_global_control
#include	"third/jWrite.c"
