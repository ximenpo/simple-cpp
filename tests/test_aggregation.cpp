#include <simple/igloo.h>
using namespace igloo;

#include	"simple/aggregation.h"

class AgTest
    : public	support_aggregation<int, AgTest> {
};

Context(aggregation_context) {
    Spec(basic_usage) {
        AgTest*	p0	= AgTest::get_instance(0);

        //	default object.
        AssertThat(AgTest::get_instance(),		Equals(p0));

        AssertThat(AgTest::get_instance_sum(),	Equals(1));
        AssertThat(p0->get_instance_id(),		Equals(0));

        AgTest*	p1	= AgTest::get_instance(1);
        AssertThat(AgTest::get_instance_sum(),	Equals(2));
        AssertThat(p1->get_instance_id(),		Equals(1));

        AssertThat(AgTest::get_instance(1),		Equals(p1));
        AssertThat(AgTest::get_instance_sum(),	Equals(2));
        AssertThat(p1->get_instance_id(),		Equals(1));

        //	default object.
        AssertThat(AgTest::get_instance(),		Equals(p0));
    }
};
