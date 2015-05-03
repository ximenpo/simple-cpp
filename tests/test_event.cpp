#include "simple/igloo.h"
using namespace igloo;

#include "simple/event.h"
#include "simple/delegate.h"

static   bool	global_func(int& n) {
    //std::cout << __FUNCTION__ << &n << std::endl;
    n++;
    return	true;
}

Context(event_usage) {
    bool	member_func(int& n) {
        //std::cout << __FUNCTION__ << &n << std::endl;
        n++;
        return	true;
    }

    Spec(func_item_test) {
        int	n	= 0;
        delegate<bool(int& n)>	func	= bind(&event_usage::member_func, this);

        func(n);
        AssertThat(n, Equals(1));
    }

    Spec(basic_usage) {
        int	n	= 0;
        event<delegate<bool(int& n)> >	evt;

        n	= 0;
        evt.notify(n);
        AssertThat(n, Equals(0));

        evt.handler().attach(bind(&event_usage::member_func, this));
        evt.handler().attach(&global_func);

        n	= 0;
        evt.notify(n);
        AssertThat(n, Equals(2));

        evt.handler().detach(bind(&event_usage::member_func, this));

        n	= 0;
        evt.notify(n);
        AssertThat(n, Equals(1));
    }

    Spec(operator_usage) {
        int	n	= 0;
        event<delegate<bool(int& n)> >	evt;

        n	= 0;
        evt(n);
        AssertThat(n, Equals(0));

        evt.handler().attach(bind(&event_usage::member_func, this));
        evt.handler().attach(&global_func);

        n	= 0;
        evt(n);
        AssertThat(n, Equals(2));

        evt.handler().detach(bind(&event_usage::member_func, this));

        n	= 0;
        evt(n);
        AssertThat(n, Equals(1));
    }

    Spec(execute_usage) {
        int	n	= 0;
        event<delegate<bool(int& n)> >	evt;

        n	= 0;
        evt.execute(n);
        AssertThat(n, Equals(0));

        evt.handler().attach(bind(&event_usage::member_func, this));
        evt.handler().attach(&global_func);

        n	= 0;
        evt.execute(n);
        AssertThat(n, Equals(1));
    }
};
