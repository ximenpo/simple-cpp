#include <igloo/igloo.h>
using namespace igloo;

#include <cstring>
#include <simple/array.h>

static	const	int	ARR_LEN	= 4;

Context(ARRAY_LENGTH_usage) {
    Spec(plain_array_length) {
        int	plain_a[ARR_LEN];
        AssertThat(ARRAY_LENGTH(plain_a), Equals(ARR_LEN));
    }

    Spec(safe_array_length) {
        safe_array<int, ARR_LEN>	safe_a;
        AssertThat(ARRAY_LENGTH(safe_a), Equals(ARR_LEN));
    }
};

Context(safe_array_usage) {
    enum	{	ARR_LEN	= 4	};
    int	plain_a[ARR_LEN];
    safe_array<int, ARR_LEN>	safe_a;

    void	Setup() {
        memset(&plain_a, 0, sizeof(plain_a));
        memset(&safe_a, 0, sizeof(safe_a));
    }

    Spec(basic_usage) {
        AssertThat(sizeof(safe_a), Equals(sizeof(plain_a)));
        AssertThat(ARRAY_LENGTH(safe_a), Equals(ARRAY_LENGTH(plain_a)));

        memset(&safe_a, 0, sizeof(safe_a));
        for(int i = 0; i < ARRAY_LENGTH(safe_a); ++i) {
            safe_a[i]	= i;
            plain_a[i]	= i;
        }

        AssertThat(memcmp(&safe_a, &plain_a, sizeof(safe_a)), Equals(0));
    }

    Spec(out_of_range_usage) {
        AssertThrows(std::out_of_range, safe_a[ARR_LEN]);
    }
};
