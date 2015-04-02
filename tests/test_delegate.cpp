#include "simple/igloo.h"
using namespace igloo;

#include "simple/delegate.h"

static	bool	global_greater_than(int n1, int n2) {
    return	n1 > n2;
}


Context(delegate_usage) {

    bool	greater_than(int n1, int n2) {
        return	n1 > n2;
    }

    Spec(basic_usage) {
        delegate<bool(int n1, int n2)>	func;

        AssertThat(bool(func), IsFalse());
        func.bind(this, &delegate_usage::greater_than);
        AssertThat(bool(func), IsTrue());

        AssertThat(func(1,2), IsFalse());
        AssertThat(func(2,2), IsFalse());
        AssertThat(func(2,1), IsTrue());

        func.bind(&global_greater_than);
        AssertThat(func(1,2), IsFalse());
        AssertThat(func(2,2), IsFalse());
        AssertThat(func(2,1), IsTrue());
    }
};
