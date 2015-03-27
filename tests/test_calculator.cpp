#include <simple/igloo.h>
using namespace igloo;

#include	<cmath>
#include	<cstring>
#include	"simple/calculator.h"

class	MyCalculator	: public calculator {
protected:
    virtual	int			do_find_variable(const char* buf, variable& var) {
        if(strncmp(buf, "x1", 2) == 0 && is_delimiter(buf[2])) {
            var	= 10;
            return	2;
        }
        return	calculator::do_find_variable(buf, var);
    }
};

Context(calculator_context) {
    Spec(basic_usage) {
        calculator	calc;
        calculator::variable	var;
        AssertThat(calc.execute("1 + 2 * (3- 4)", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(-1, 0.001));
    }

    Spec(assign_usage) {
        calculator	calc;
        calculator::variable	var;
        AssertThat(calc.execute("ret = 1 + 2 * (3- 4)", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(-1, 0.001));
        AssertThat(calc.get_variable("ret", var),				IsTrue());
        AssertThat(var,		EqualsWithDelta(-1, 0.001));
    }

    Spec(set_variable_usage) {
        calculator	calc;
        calculator::variable	var;
        calc.set_variable("x", 10);
        AssertThat(calc.execute("ret = x + 2 * (3- 4)", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(8, 0.001));
        AssertThat(calc.get_variable("ret", var),				IsTrue());
        AssertThat(var,		EqualsWithDelta(8, 0.001));
    }

    Spec(func_usage) {
        calculator	calc;
        calculator::variable	var;
        AssertThat(calc.execute("sin(0.5) + 1", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta((sin(0.5) + 1), 0.001));
    }

    Spec(inherited_usage) {
        MyCalculator	calc;
        calculator::variable	var;
        AssertThat(calc.execute("x1 + 1", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(11, 0.0001));
    }
};
