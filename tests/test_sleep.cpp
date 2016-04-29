#include "simple/igloo.h"
using namespace igloo;

#include "simple/timestamp.h"
#include "simple/sleep.h"

Context(sleep_usage) {
    Spec(sleep_seconds_usage) {
        unsigned long	begin_stamp	= timestamp_tickcount();
        sleep_seconds(1);
        unsigned long	end_stamp	= timestamp_tickcount();
        AssertThat(end_stamp - begin_stamp,	EqualsWithDelta(1000, 100));
    }
};
