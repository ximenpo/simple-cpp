
#include	"simple/third/jWrite.h"

//------------------------------------------
// jwErrorToString
// - returns string describing error code
//
const char *jwErrorToString( int err ) {
    switch( err ) {
    case JWRITE_OK:
        return "OK";
    case JWRITE_BUF_FULL:
        return "output buffer full";
    case JWRITE_NOT_ARRAY:
        return "tried to write Array value into Object";
    case JWRITE_NOT_OBJECT:
        return "tried to write Object key/value into Array";
    case JWRITE_STACK_FULL:
        return "array/object nesting > JWRITE_STACK_DEPTH";
    case JWRITE_STACK_EMPTY:
        return "stack underflow error (too many 'end's)";
    case JWRITE_NEST_ERROR:
        return "nesting error, not all objects closed when jwClose() called";
    }
    return "Unknown error";
}
