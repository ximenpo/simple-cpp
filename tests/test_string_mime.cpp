#include "simple/igloo.h"
using namespace igloo;

#include "simple/string.h"

Context(string_mime_context) {
    Spec(base64_usage) {
        std::string		s1, s2, s3;
        s1	= "I'm XiMenPo";
        s2	= string_base64_encode(s1);
        s3	= string_base64_decode(s2);
        AssertThat(string_is_base64(s2), IsTrue());
        AssertThat(s3,	Equals(s1));
    }

    Spec(base64_chinese_encoding) {
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

    Spec(qp_basic_usage) {
        std::string		s1, s2, s3;
        s1	= "I'm XiMenPo";
        s2	= string_qp_encode(s1);
        s3	= string_qp_decode(s2);
        AssertThat(s3,	Equals(s1));
    }

    Spec(qp_chinese_encoding) {
#if	defined(_MSC_VER)
#include	"test_data/str_gb2312.inc"
#else
#include	"test_data/str_utf8.inc"
#endif
        std::string encode = string_qp_encode(str);
        std::string decode = string_qp_decode(encode);
        AssertThat(str,		Equals(decode));
#if	defined(_MSC_VER)
        AssertThat(encode,	Equals("XiMenPo=CA=C7Simple=B5=C4=D2=F4=D2=EB=C3=FB=A1=A3"));
#else
        AssertThat(encode,	Equals("XiMenPo=E6=98=AFSimple=E7=9A=84=E9=9F=B3=E8=AF=91=E5=90=8D=E3=80=82"));
#endif
    }
};
