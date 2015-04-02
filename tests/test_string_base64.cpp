#include "simple/igloo.h"
using namespace igloo;

#include "simple/string.h"

Context(base64_context) {
    Spec(basic_usage) {
        std::string		s1, s2, s3;
        s1	= "I'm XiMenPo";
        s2	= string_base64_encode(s1);
        s3	= string_base64_decode(s2);
        AssertThat(string_is_base64(s2), IsTrue());
        AssertThat(s3,	Equals(s1));
    }

    Spec(chinese_encoding) {
#if	defined(_MSC_VER)
#include	"test_data/str_gb2312.inc"
#else
#include	"test_data/str_utf8.inc"
#endif
        std::string encode = string_base64_encode(str);
        std::string decode = string_base64_decode(encode);
        AssertThat(str,		Equals(decode));
#if	defined(_MSC_VER)
        AssertThat(encode,	Equals("WGlNZW5Qb8rHU2ltcGxltcTS9NLrw/uhow=="));
#else
        AssertThat(encode,	Equals("WGlNZW5Qb+aYr1NpbXBsZeeahOmfs+ivkeWQjeOAgg=="));
#endif
    }
};
