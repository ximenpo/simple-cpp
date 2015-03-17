#include <igloo/igloo.h>
using namespace igloo;

#include <simple/binary.h>

Context(binary_usage) {
    Spec(basic_usage) {
        int	n	= B01010101;
        AssertThat(n,			Equals(85));
        AssertThat(B01010101,	Equals(85));
        AssertThat(85,			Equals(B01010101));
        AssertThat(B1,			Equals(1));
        AssertThat(B01,			Equals(1));
        AssertThat(B001,		Equals(1));
    }
};
