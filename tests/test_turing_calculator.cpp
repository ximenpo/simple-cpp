#include "simple/igloo.h"
using namespace igloo;

#include	<cmath>
#include	<cstring>
#include	"simple/turing_calculator.h"

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

};
