#include <igloo/igloo.h>
using namespace igloo;

#include <simple/rand.h>

Context(rand_usage) {
    Spec(rand_normal_distribution_usage) {
        int	nums[3]	= {0};
        for(int i = 0; i < 1000; ++i) {
            nums[rand_normal_distribution(0,1,2,3)]++;
        }

        AssertThat(5 * (nums[0] + nums[2]) <  nums[1], IsTrue());
    }
};
