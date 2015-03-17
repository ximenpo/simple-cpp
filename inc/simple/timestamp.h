#ifndef TIMESTAMP_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TIMESTAMP_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <climits>

inline	unsigned long	timestamp_tickcount();

#if	defined(_WIN32)
extern	"C"	unsigned long	__stdcall	GetTickCount(void);
inline	unsigned long	timestamp_tickcount() {
    return	GetTickCount();
}
#elif defined(__APPLE__) && defined(__MACH__)	// mac os
#	include <float.h>
#	include <mach/mach_time.h>
inline	unsigned long	timestamp_tickcount() {
    static	double	ticksToMicroSeconds	= 0.0;
    if( ticksToMicroSeconds <= DBL_EPSILON ) {
        mach_timebase_info_data_t	sTimebaseInfo;
        mach_timebase_info(&sTimebaseInfo);
        ticksToMicroSeconds = (double)sTimebaseInfo.numer / sTimebaseInfo.denom / 1000000.0;
    }
    return	(unsigned long)(mach_absolute_time() * ticksToMicroSeconds);
}
#else											// linux
#	include	<time.h>
inline	unsigned long	timestamp_tickcount() {
    struct timespec ts;
#	if	defined(CLOCK_MONOTONIC)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#	else
    clock_gettime(CLOCK_REALTIME, &ts);
#	endif
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif

//
//	时间戳工具类，用来获取从某个时刻到当前经过的时间戳（秒数）
//
//	假设：GetTimestamp函数在49天内一定会被调用过一次
//
class	timestamp {
public:
    explicit	timestamp(double timestamp_begin = 0.0) {
        reset(timestamp_begin);
    }

public:
    //	重新启动时间戳计数
    void		reset(double timestamp_begin = 0.0) {
        tickcount_begin_	= timestamp_tickcount();
        tickcount_last_		= tickcount_begin_;

        timestamp_begin_	= timestamp_begin;
        timestamp_			= timestamp_begin_;
    }

    //	返回以秒为单位的时间戳
    double		now() {
        unsigned long	dwTickCount	= timestamp_tickcount();

        if(dwTickCount == tickcount_last_) {
            return timestamp_;
        }

        if(dwTickCount > tickcount_begin_) {
            timestamp_	= timestamp_begin_ + (dwTickCount -	tickcount_begin_) /	1000.0;
            if(dwTickCount >= tickcount_begin_ + 24	* 60 * 60 *	1000) {
                // 超过了一天，则更新起始值
                // 在临界点上tickcount_begin_接近最大值，则更新操作等待TickCount归0后处理
                timestamp_begin_	= timestamp_;
                tickcount_begin_	= dwTickCount;
            }
        } else {
            timestamp_		= timestamp_begin_ + (ULONG_MAX - tickcount_begin_ + dwTickCount) / 1000.0f;

            timestamp_begin_	= timestamp_;
            tickcount_begin_	= dwTickCount;
        }

        tickcount_last_	= dwTickCount;
        return	timestamp_;
    }

private:
    volatile	unsigned long		tickcount_last_;		//	最后一次TickCount值
    volatile	unsigned long		tickcount_begin_;		//	初始TickCount值

    volatile	double				timestamp_;				//	最后一次时间戳值
    volatile	double				timestamp_begin_;		//	初始时间戳值
};

#endif
