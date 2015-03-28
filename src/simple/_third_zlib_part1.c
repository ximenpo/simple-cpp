
#if		defined(_MSC_VER)
#	pragma	warning(disable: 4996)
#endif

#if     !defined(_WIN32)
#   include <unistd.h>
#endif

#include	"simple/third/zlib/compress.c"
#include	"simple/third/zlib/uncompr.c"
#include	"simple/third/zlib/inflate.c"
#include	"simple/third/zlib/adler32.c"
#include	"simple/third/zlib/gzlib.c"
