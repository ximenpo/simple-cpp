
#if			defined(JW_GLOBAL_CONTROL_STRUCT)
#	undef	JW_GLOBAL_CONTROL_STRUCT
#endif

#define		JW_GLOBAL_CONTROL_STRUCT
#define		jwErrorToString		jwErrorToString_with_global_control
#include	"third/jWrite.c"
