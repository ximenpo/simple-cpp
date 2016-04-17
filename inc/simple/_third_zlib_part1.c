
#if		defined(_MSC_VER)
#	pragma	warning(disable: 4996)
#endif

#if     !defined(_WIN32)
#   include <unistd.h>
#endif

//
//	disable gcc compiler warnings.
//
#ifdef __GNUC__
#	pragma  GCC diagnostic ignored  "-Wshift-negative-value"
#endif

#include	"_third/zlib/compress.c"
#include	"_third/zlib/uncompr.c"
#include	"_third/zlib/inflate.c"
#include	"_third/zlib/adler32.c"
#include	"_third/zlib/gzlib.c"
