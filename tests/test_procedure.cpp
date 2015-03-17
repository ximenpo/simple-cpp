#include <igloo/igloo.h>
using namespace igloo;

#include <simple/procedure.h>

static	procedure_label		global_lbl;
Context(procedure_context_usage) {
    static	int		simple_procedure(procedure_context* context) {
        PROCEDURE_BEGIN(context);

        PROCEDURE_YIELD_(1);

        PROCEDURE_YIELD_(2);

        PROCEDURE_LABEL_(&global_lbl);
        PROCEDURE_YIELD_(3);

        PROCEDURE_YIELD_(4);

        PROCEDURE_END_(5);
    }

    static	int		simple_loop_procedure(procedure_context* context) {
        PROCEDURE_BEGIN_RUNLOOP(context);

        PROCEDURE_YIELD_(1);

        PROCEDURE_YIELD_(2);

        PROCEDURE_END_(0);
    }

    static	int		complex_procedure(procedure_context* context) {
        struct Data : public procedure_data	{
            int	n;
        };

        PROCEDURE_BEGIN_(context, Data, data);

        for(data->n = 1; data->n <= 4; data->n++) {
            PROCEDURE_YIELD_(data->n);
        }

        PROCEDURE_END_(0);
    }

    Spec(basic_usage) {
        procedure_context	ctx;

        AssertThat(ctx.is_running(), IsFalse());

        AssertThat(simple_procedure(&ctx), Equals(1));
        AssertThat(simple_procedure(&ctx), Equals(2));
        AssertThat(simple_procedure(&ctx), Equals(3));
        AssertThat(simple_procedure(&ctx), Equals(4));

        AssertThat(ctx.is_running(), IsTrue());
        AssertThat(ctx.is_finished(), IsFalse());
        AssertThat(simple_procedure(&ctx), Equals(5));
        AssertThat(ctx.is_running(), IsFalse());
        AssertThat(ctx.is_finished(), IsTrue());

        AssertThat(simple_procedure(&ctx), Equals(5));
        AssertThat(ctx.is_finished(), IsTrue());

        ctx.goto_label(&global_lbl);
        AssertThat(simple_procedure(&ctx), Equals(3));
        AssertThat(ctx.is_running(), IsTrue());
        AssertThat(ctx.is_finished(), IsFalse());
    }

    Spec(with_local_data) {
        procedure_context	ctx;

        AssertThat(complex_procedure(&ctx), Equals(1));
        AssertThat(complex_procedure(&ctx), Equals(2));
        AssertThat(complex_procedure(&ctx), Equals(3));
        AssertThat(complex_procedure(&ctx), Equals(4));

        AssertThat(complex_procedure(&ctx), Equals(0));

        AssertThat(ctx.is_running(), IsFalse());
        AssertThat(ctx.is_finished(), IsTrue());
    }

    Spec(RUNLOOP_usage) {
        procedure_context	ctx;

        AssertThat(ctx.is_running(), IsFalse());

        AssertThat(simple_loop_procedure(&ctx), Equals(1));
        AssertThat(simple_loop_procedure(&ctx), Equals(2));

        AssertThat(ctx.is_running(), IsTrue());
        AssertThat(ctx.is_finished(), IsFalse());
        AssertThat(simple_loop_procedure(&ctx), Equals(0));
        AssertThat(ctx.is_running(), IsFalse());
        AssertThat(ctx.is_finished(), IsTrue());

        // loop again
        AssertThat(simple_loop_procedure(&ctx), Equals(1));
        AssertThat(simple_loop_procedure(&ctx), Equals(2));
        AssertThat(ctx.is_running(), IsTrue());
        AssertThat(ctx.is_finished(), IsFalse());
    }

    Spec(save_label_usage) {
        procedure_context	ctx;
        procedure_label		lbl;

        AssertThat(ctx.is_running(), IsFalse());

        AssertThat(simple_loop_procedure(&ctx), Equals(1));
        ctx.save_label(&lbl);
        AssertThat(simple_loop_procedure(&ctx), Equals(2));
        ctx.goto_label(&lbl);
        AssertThat(simple_loop_procedure(&ctx), Equals(2));
    }
};

Context(procedure_context_ex_usage) {
    static	bool		procedure_0(procedure_context_ex* ctx, int& data) {
        PROCEDUREEX_BEGIN(ctx);

        data	= 1;
        PROCEDUREEX_YIELD();

        while(!procedure_1(ctx, data)) {
            PROCEDUREEX_YIELD();
        }

        data	= 2;
        PROCEDUREEX_YIELD();

        PROCEDUREEX_END();
    }

    static	bool		procedure_1(procedure_context_ex* ctx, int& data) {
        PROCEDUREEX_BEGIN(ctx);

        data	= 11;
        PROCEDUREEX_YIELD();

        while(!procedure_2(ctx, data)) {
            PROCEDUREEX_YIELD();
        }

        data	= 12;
        PROCEDUREEX_YIELD();

        PROCEDUREEX_END();
    }

    static	bool		procedure_2(procedure_context_ex* ctx, int& data) {
        PROCEDUREEX_BEGIN(ctx);

        data	= 21;
        PROCEDUREEX_YIELD();

        data	= 22;
        PROCEDUREEX_YIELD();

        PROCEDUREEX_END();
    }

    Spec(basic_usage) {
        procedure_context_ex	ctx;
        int						data;

        AssertThat(ctx.is_running(), IsFalse());

        procedure_0(&ctx, data);
        AssertThat(data, Equals(1));
        procedure_0(&ctx, data);
        AssertThat(data, Equals(11));
        procedure_0(&ctx, data);
        AssertThat(data, Equals(21));
        procedure_0(&ctx, data);
        AssertThat(data, Equals(22));
        procedure_0(&ctx, data);
        AssertThat(data, Equals(12));
        procedure_0(&ctx, data);
        AssertThat(data, Equals(2));

        AssertThat(ctx.is_running(), IsTrue());
        AssertThat(ctx.is_finished(), IsFalse());
    }
};
