#include "simple/igloo.h"
using namespace igloo;

#include "simple/switch.h"

Context(multi_switch_usage) {
    bool	gotTrue()	{
        return true;
    }
    bool	gotFalse()	{
        return false;
    }

    Spec(basic_usage) {
        bool	is_ok	= false;
        switch(FLAG3(gotTrue(), gotFalse(), 1 == 1)) {
        case COND3(T, F, F):
            is_ok	= false;
            break;
        case COND3(T, F, T):
            is_ok	= true;
            break;
        case COND3(F, X, X):
            is_ok	= false;
            break;
        }

        AssertThat(is_ok, IsTrue());
    }
};
