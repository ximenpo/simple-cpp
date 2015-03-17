#ifndef SLEEP_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define SLEEP_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

inline	void	sleep_seconds(int seconds);

#if	defined(_WIN32)
extern	"C"	void	__stdcall	Sleep(unsigned long dwMilliseconds);
inline	void	sleep_seconds(int seconds) {
    Sleep(1000 * seconds);
}
#else
#include	"unistd.h"
inline	void	sleep_seconds(int seconds) {
    sleep(seconds);
}
#endif

#endif
