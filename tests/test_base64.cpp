#include <igloo/igloo.h>
using namespace igloo;

#include "simple/base64.h"

Context(base64_context) {
    Spec(basic_usage) {
        std::string		s1, s2, s3;
        s1	= "I'm XiMenPo";
        s2	= base64_encode(s1);
        s3	= base64_decode(s2);
        AssertThat(is_base64(s2),	IsTrue());
        AssertThat(s3,	Equals(s1));
    }
};
