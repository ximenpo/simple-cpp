#include "simple/igloo.h"
using namespace igloo;

#include "simple/combination.h"
#include "simple/delegate.h"

class	combination_context;

Context(combination_context) {
    size_t	index;
    void	SetUp() {
        index	= 0;
    }

    bool	Check_4_of_6(char* begin, char* end) {
        static	const char* result[]	= {
            "1234",
            "1235",
            "1236",
            "1245",
            "1246",
            "1256",
            "1345",
            "1346",
            "1356",
            "1456",
            "2345",
            "2346",
            "2356",
            "2456",
            "3456",
        };
        AssertThat(begin,	Equals(result[index]));
        bool	ret	= 0 == strcmp(begin, result[index]);
        index++;
        return	ret;
    }

    Spec(next_combination_usage) {
        char ca[]="123456";
        char cb[]="1234";

        AssertThat(Check_4_of_6(cb, 0),		IsTrue());
        while(next_combination(ca,ca+6,cb,cb+4)) {
            AssertThat(Check_4_of_6(cb, 0),		IsTrue());
        }
        AssertThat(index,	Equals(15));
    }

    Spec(recursive_combination_usage) {
        char ca[]="123456";
        char cb[]="1234";

        recursive_combination((char*)ca, ca+6, 0, (char*)cb, cb+4, 0, 6-4, bind(&combination_context::Check_4_of_6, this));
        AssertThat(index,	Equals(15));
    }
};
