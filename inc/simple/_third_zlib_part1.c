
#if		defined(_MSC_VER)
#	pragma	warning(disable: 4996)
#endif

#if     !defined(_WIN32)
#   include <unistd.h>
#endif

#include	"third/zlib/compress.c"
#include	"third/zlib/uncompr.c"
#include	"third/zlib/inflate.c"
#include	"third/zlib/adler32.c"
#include	"third/zlib/gzlib.c"
