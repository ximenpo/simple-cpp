#include <simple/igloo.h>
using namespace igloo;

#include <simple/timestamp.h>
#include <simple/sleep.h>

Context(timestamp_usage) {
    Spec(usage) {
        timestamp	stamp;
        AssertThat(stamp.now(),	EqualsWithDelta(0.0, 0.02));//18ms

        sleep_seconds(1);
        AssertThat(stamp.now(),	EqualsWithDelta(1.0, 0.02));//18ms
    }
};
