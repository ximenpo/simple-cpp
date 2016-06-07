#ifndef STDINT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STDINT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#   ifndef  __STDC_LIMIT_MACROS
#       define  __STDC_LIMIT_MACROS
#   endif
#   include "_third/msinttypes/stdint.h"
#elif defined(_MSC_VER)
#		pragma warning (push)
#		pragma warning (disable : 4005)
#			include <stdint.h>
#		pragma warning (pop)
#else
#	include <stdint.h>
#endif

#endif
