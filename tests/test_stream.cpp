#include "simple/igloo.h"
using namespace igloo;

#include <sstream>
#include "simple/stream.h"

Context(stream_usage) {
    Spec(mandatory_input_usage) {
        static const char* inputs[]	= {
            "(1,1)",
            "( 1, 1)",
            " (1,1) ",
            " ( 1 , 1 ) ",
            " ( 1 , 1 ) )",
        };
        for(size_t i = 0; i < sizeof(inputs)/sizeof(inputs[0]); ++i) {
            int	x = 0, y = 0;
            std::istringstream	is(inputs[i]);
            AssertThat(!(is >> mandatory_input(" (") >> x >> mandatory_input(" ,") >> y >> mandatory_input(" )")),	IsFalse());
            AssertThat(x,	Equals(1));
            AssertThat(y,	Equals(1));
        }

        {
            int	x = 0, y = 0;
            std::istringstream	is("(1  2)");
            AssertThat(!(is >> mandatory_input(" (") >> x >> mandatory_input(" ,") >> y >> mandatory_input(" )")),	IsTrue());
        }
    }

    Spec(dummy_usage) {
        static const char* inputs[]	= {
            "(1,1)",
            "( 1, 1)",
            " (1,1) ",
            " ( 1 , 1 ) ",
            " ( 1 , 1 ) )",
        };
        for(size_t i = 0; i < sizeof(inputs)/sizeof(inputs[0]); ++i) {
            int	x = 0, y = 0;
            std::istringstream	is(inputs[i]);
            AssertThat(!(is >> dummy(" (") >> x >> dummy(" ,") >> y >> dummy(" )")),	IsFalse());
            AssertThat(x,	Equals(1));
            AssertThat(y,	Equals(1));
        }

        {
            int	x = 0, y = 0;
            std::istringstream	is("(1  2)");
            AssertThat(!(is >> dummy(" (") >> x >> dummy(" ,") >> y >> dummy(" )")),	IsTrue());
        }
    }
};
