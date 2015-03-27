#include <simple/igloo.h>
using namespace igloo;

#include <simple/bit.h>
#include <simple/binary.h>

Context(bit_usage) {
    Spec(bit_count_usage) {
        AssertThat(bit_count(B00000000),	Equals(0));
        AssertThat(bit_count(B01010101),	Equals(4));
        AssertThat(bit_count(B11011101),	Equals(6));
        AssertThat(bit_count(B11111111),	Equals(8));
    }

    Spec(bit_lowest_one_index_usage) {
        AssertThat(bit_lowest_one_index(B00000000),	Equals(size_t(BITS_COUNT)));
        AssertThat(bit_lowest_one_index(B00000010),	Equals(1));
        AssertThat(bit_lowest_one_index(B01000000),	Equals(6));
    }

    Spec(bit_token_usage) {
        enum { COUNT = 1024 };

        bit_token<COUNT>      tokens;			// bucket to keep track of available bits

        for(int i = 0; i < COUNT; ++i) {
            size_t index = tokens.get_token();	// find first zero bit in table
            if (index >= tokens.size()) {		// out of range
                break;
            }
            tokens.set(index, true);			// update bit in bucket
            AssertThat(index,	Equals(i));
        }

        for(int i = 0; i < COUNT; ++i) {
            size_t	r	= rand() % COUNT;
            tokens.set(r, false);				// mark bucket as available

            size_t index = tokens.get_token();	// find first zero bit in table
            tokens.set(index, true);
            AssertThat(index,	Equals(r));
        }
    }
};
