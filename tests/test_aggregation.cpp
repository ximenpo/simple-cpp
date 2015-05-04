#include "simple/igloo.h"
using namespace igloo;

#include	"simple/aggregation.h"

class AgTest
    : public	support_aggregation<int, AgTest> {
};

Context(aggregation_context) {
    Spec(basic_usage) {
        AgTest*	p0	= AgTest::instance(0);

        //	default object.
        AssertThat(AgTest::instance(),		Equals(p0));

        AssertThat(AgTest::instance_sum(),	Equals(1));
        AssertThat(p0->instance_id(),		Equals(0));

        AgTest*	p1	= AgTest::instance(1);
        AssertThat(AgTest::instance_sum(),	Equals(2));
        AssertThat(p1->instance_id(),		Equals(1));

        AssertThat(AgTest::instance(1),		Equals(p1));
        AssertThat(AgTest::instance_sum(),	Equals(2));
        AssertThat(p1->instance_id(),		Equals(1));

        //	default object.
        AssertThat(AgTest::instance(),		Equals(p0));
    }
};
