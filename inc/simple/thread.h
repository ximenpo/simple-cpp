#ifndef THREAD_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define THREAD_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

extern	"C" {
#	include	“_third/simplethread/simplethread.h”
}

#if	!defined(THREAD_STORAGE)
#	if		defined(_MSC_VER)
#		define	THREAD_STORAGE		__declspec( thread )
#	elif	defined(__GNUC__)
#		define	THREAD_STORAGE		__thread
#	else
#		define	THREAD_STORAGE
#	endif
#endif

#endif
