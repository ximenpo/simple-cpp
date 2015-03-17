#include <igloo/igloo.h>
using namespace igloo;

#include <simple/guard.h>

static	void	recursive_invoke(int& n) {
    if(n > 1000) {
        return;
    }

    GUARD_RECURSIVE_INVOKE_TIMES(10);

    n++;
    recursive_invoke(n);
}

Context(guard_usage) {
    Spec(GUARD_RECURSIVE_INVOKE_TIMES_usage) {
        int	n	= 0;
        AssertThrows(std::logic_error,	recursive_invoke(n));
        AssertThat(n,	Equals(10));
    }
};
