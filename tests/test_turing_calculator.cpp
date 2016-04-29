#include "simple/igloo.h"
using namespace igloo;

#include	<cmath>
#include	<cstring>
#include    "simple/string.h"
#include	"simple/turing_calculator.h"

class	MyTuringCalculator	: public turing_calculator {
public:
    std::string     name;

private:
    void    set_name(size_t idx_name, int times) {
        this->name  = string_format("%s - %d", this->get_str_variable(idx_name), times);
    }

protected:
    virtual	variable	do_find_function(const char* buf, int& move) {
        variable    x,y;
        if(0 == strncmp(buf, "set_name(", 9)) {
            move = 9 + fetch_func_params(buf+9, 2, x, y);
            this->set_name(size_t(x), int(y));
            return  variable(1);
        }
        return	turing_calculator::do_find_function(buf, move);
    }
};

Context(turing_calculator_context) {
    Spec(check_str_var) {
        turing_calculator   calc;

        const char* media[] = {
            "@s1=",
            "@s2=ximenpo",
            "@s3 adf = simple ",
            "@s1 = great! ",
            "",
        };

        AssertThat(
            calc.load_instructions(media, sizeof(media)/sizeof(media[0])),
            IsTrue()
        );

        calculator::variable	var;

        AssertThat(calc.fetch_variable("@s1", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(0, 0.001));
        AssertThat(calc.get_str_variable("@s1"),	Equals(" great! "));

        AssertThat(calc.fetch_variable("@s2", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(1, 0.001));
        AssertThat(calc.get_str_variable("@s2"),	Equals("ximenpo"));

        AssertThat(calc.fetch_variable("@s3", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(2, 0.001));
        AssertThat(calc.get_str_variable("@s3"),	Equals(" simple "));

        AssertThat(calc.fetch_variable("@s4", var),	IsFalse());
        AssertThat(calc.get_str_variable("@s4"),	Equals((void*)NULL));
    }

    Spec(check_label_var) {
        turing_calculator   calc;

        const char* media[] = {
            ":begin sdfa",
            ":begin",
            ":begin ",
            " :begin asd fsf ",
            ":end  ",
        };

        AssertThat(
            calc.load_instructions(media, sizeof(media)/sizeof(media[0])),
            IsTrue()
        );

        calculator::variable	var;

        AssertThat(calc.fetch_variable(":begin", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(0, 0.001));
        AssertThat(var,		Equals(0));

        AssertThat(calc.fetch_variable(":end", var),	IsTrue());
        AssertThat(var,		EqualsWithDelta(4, 0.001));
        AssertThat(var,		Equals(4));

        AssertThat(calc.fetch_variable("$xxx", var),	IsFalse());
    }

    Spec(basic_usage) {
        MyTuringCalculator  calc;

        const char* media[] = {
            "@s1 =ximenpo",
            ":begin",
            "v1 = 1",
            ":loop",
            "v1 = v1 + 1",
            "# comment here",
            "GOTO_IF(:end, GT(v1, 5))",
            "set_name(@s1, v1)",
            "GOTO(:loop)",
            ":end  ",
        };

        AssertThat(
            calc.load_instructions(media, sizeof(media)/sizeof(media[0])),
            IsTrue()
        );

        turing_machine      vm;
        vm.instruction_executor = bind(&turing_calculator::execute_instruction, &calc);

        vm.start();
        vm.run(false);

        calculator::variable	var;
        AssertThat(calc.fetch_variable("v1", var),	IsTrue());
        AssertThat(var,         EqualsWithDelta(6, 0.001));
        AssertThat(var,         Equals(6));

        AssertThat(calc.name,   Equals("ximenpo - 5"));
    }

    Spec(stop_usage) {
        MyTuringCalculator  calc;

        const char* media[] = {
            "@s1 =ximenpo",
            ":begin",
            "v1 = 1",
            ":loop",
            "v1 = v1 + 1",
            "GOTO_IF(:end, GT(v1, 5))",
            "STOP()",                       // <--- NOTE HERE
            "set_name(@s1, v1)",
            "GOTO(:loop)",
            ":end  ",
        };

        AssertThat(
            calc.load_instructions(media, sizeof(media)/sizeof(media[0])),
            IsTrue()
        );

        turing_machine      vm;
        vm.instruction_executor = bind(&turing_calculator::execute_instruction, &calc);

        vm.start();
        vm.run(false);

        calculator::variable	var;
        AssertThat(calc.fetch_variable("v1", var),	IsTrue());
        AssertThat(var,         EqualsWithDelta(2, 0.001));
        AssertThat(var,         Equals(2));

        AssertThat(calc.name,   Equals(""));
    }

};
