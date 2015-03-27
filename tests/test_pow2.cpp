#include <simple/igloo.h>
using namespace igloo;

#include <simple/pow2.h>

Context(pow2_usage) {
    Spec(is_pow2_usage) {
        AssertThat(is_pow2(0),	IsFalse());
        AssertThat(is_pow2(1),	IsTrue());
        AssertThat(is_pow2(2),	IsTrue());
        AssertThat(is_pow2(3),	IsFalse());
        AssertThat(is_pow2(64),	IsTrue());
        AssertThat(is_pow2(65),	IsFalse());
    }

    Spec(next_pow2_usage) {
        AssertThat(pow2_next(0),	Equals(1));
        AssertThat(pow2_next(1),	Equals(2));
        AssertThat(pow2_next(2),	Equals(4));
        AssertThat(pow2_next(3),	Equals(4));
        AssertThat(pow2_next(4),	Equals(8));
        AssertThat(pow2_next(32),	Equals(64));
        AssertThat(pow2_next(47),	Equals(64));
        AssertThat(pow2_next(64),	Equals(128));
        AssertThat(pow2_next(65),	Equals(128));
    }

    Spec(prev_pow2_usage) {
        AssertThat(pow2_prev(0),	Equals(1));
        AssertThat(pow2_prev(1),	Equals(1));
        AssertThat(pow2_prev(2),	Equals(1));
        AssertThat(pow2_prev(3),	Equals(2));
        AssertThat(pow2_prev(4),	Equals(2));
        AssertThat(pow2_prev(32),	Equals(16));
        AssertThat(pow2_prev(47),	Equals(32));
        AssertThat(pow2_prev(64),	Equals(32));
        AssertThat(pow2_prev(65),	Equals(64));
    }

    Spec(near_pow2_usage) {
        AssertThat(pow2_near(0),	Equals(1));
        AssertThat(pow2_near(1),	Equals(1));
        AssertThat(pow2_near(2),	Equals(2));
        AssertThat(pow2_near(3),	Equals(4));
        AssertThat(pow2_near(4),	Equals(4));
        AssertThat(pow2_near(32),	Equals(32));
        AssertThat(pow2_near(47),	Equals(32));
        AssertThat(pow2_near(64),	Equals(64));
        AssertThat(pow2_near(65),	Equals(64));
    }
};
