#include "simple/igloo.h"
using namespace igloo;

#include "simple/procedure.h"
#include "simple/timer.h"

Context(timer_context) {
    bool	invoked;
    int		stage;

    void	SetUp() {
        invoked	= false;
        stage	= 0;
    }

    void	simple_callback_1() {
        invoked	= true;
    }

    void	simple_callback_2(void* param) {
        invoked	= true;
    }

    void	simple_callback_3(any_ptr param) {
        invoked	= true;
    }

    bool	procedure_callback(procedure_context* ctx, procedure_timing* timing) {
        PROCEDURE_BEGIN(ctx);
        stage	= 1;
        PROCEDURE_YIELD_(false);
        stage	= 2;
        PROCEDURE_SLEEP_(timing, 1.0, false);
        stage	= 3;
        PROCEDURE_END_(true);
    }

    bool	procedure_ex_callback(procedure_context_ex* ctx, procedure_timing* timing) {
        PROCEDUREEX_BEGIN(ctx);
        stage	= 1;
        PROCEDUREEX_YIELD();
        stage	= 2;
        PROCEDUREEX_SLEEP(timing, 1.0);
        stage	= 3;
        PROCEDUREEX_END();
    }

    Spec(basic_usage) {
        timer	t;
        t.execute(bind(&timer_context::simple_callback_3, this), 2.0f);
        AssertThat(invoked,	IsFalse());
        t.update(1.0f);
        AssertThat(invoked,	IsFalse());
        t.update(1.0f);
        AssertThat(invoked,	IsTrue());
    }

    Spec(erase_usage) {
        timer	t;
        unsigned int n	= t.execute(bind(&timer_context::simple_callback_3, this), 2.0f);
        AssertThat(invoked,	IsFalse());
        t.update(1.0f);
        AssertThat(invoked,	IsFalse());
        t.erase(n);
        t.update(1.0f);
        AssertThat(invoked,	IsFalse());
        AssertThat(t.size(),Equals(0));
    }

    Spec(no_param_usage) {
        timer	t;
        t.execute(bind(&timer_context::simple_callback_1, this), 2.0f);
        AssertThat(invoked,	IsFalse());
        t.update(1.0f);
        AssertThat(invoked,	IsFalse());
        t.update(1.0f);
        AssertThat(invoked,	IsTrue());
    }

    Spec(param_void_star_usage) {
        timer	t;
        t.execute(bind(&timer_context::procedure_callback, this));
        AssertThat(stage,	Equals(0));
        t.update(0.1f);
        AssertThat(stage,	Equals(1));
        t.update(0.1f);
        AssertThat(stage,	Equals(2));
        t.update(0.1f);
        AssertThat(stage,	Equals(2));
        t.update(0.901f);
        AssertThat(stage,	Equals(3));
        t.update(0.1f);
        AssertThat(t.size(),Equals(0));
    }

    Spec(procedure_usage) {
        timer	t;
        t.execute(bind(&timer_context::procedure_ex_callback, this));
        AssertThat(stage,	Equals(0));
        t.update(0.1f);
        AssertThat(stage,	Equals(1));
        t.update(0.1f);
        AssertThat(stage,	Equals(2));
        t.update(0.1f);
        AssertThat(stage,	Equals(2));
        t.update(0.901f);
        AssertThat(stage,	Equals(3));
        t.update(0.1f);
        AssertThat(t.size(),Equals(0));
    }
};
