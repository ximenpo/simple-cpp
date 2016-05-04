#include "simple/igloo.h"
using namespace igloo;

#include <stdio.h>

#include <iterator>
#include "simple/string.h"

Context(string_usage) {
    Spec(trim_const_strings_usage) {
        AssertThat(string_ltrim(" \t\nOK"),		Equals("OK"));
        AssertThat(string_rtrim("OK \t\n"),		Equals("OK"));
        AssertThat(string_trim(" \t\nOK \t\n"),	Equals("OK"));
    }

    Spec(trim_strings_usage) {
        std::string	s1(" \t\nOK"), s2("OK \t\n"), s3(" \t\nOK \t\n");

        string_ltrim(s1);
        AssertThat(s1,	Equals("OK"));
        string_rtrim(s2);
        AssertThat(s2,	Equals("OK"));
        string_trim(s3);
        AssertThat(s3,	Equals("OK"));
    }

    Spec(case_convert_const_strings_usage) {
        AssertThat(string_toupper("OK, I'm XiMenPo"),		Equals("OK, I'M XIMENPO"));
        AssertThat(string_tolower("OK, I'm XiMenPo"),		Equals("ok, i'm ximenpo"));
    }

    Spec(case_convert_usage) {
        std::string	s1, s2;
        s1	= s2	= "OK, I'm XiMenPo";

        string_toupper(s1);
        AssertThat(s1,		Equals("OK, I'M XIMENPO"));

        string_tolower(s2);
        AssertThat(s2,		Equals("ok, i'm ximenpo"));
    }

    Spec(hex_usage) {
        for(unsigned int i = 0; i < 0x100; ++i) {
            char	buf[4]	= {0};
            sprintf(buf, "%02X", i);
            AssertThat(buf,	Equals(string_hex((unsigned char)i)));
        }

        for(unsigned int i = 0; i < 0x100; ++i) {
            char	buf[4]	= {0};
            sprintf(buf, "%02x", i);
            AssertThat(buf,	Equals(string_hex((unsigned char)i, true)));
        }
    }

    Spec(format_usage) {
        std::string	s1;
        string_format(s1, "%s, %d", "XiMenPo", 35);
        AssertThat(s1,		Equals("XiMenPo, 35"));

        AssertThat(string_format("%s, %d", "XiMenPo", 35),		Equals("XiMenPo, 35"));
    }

    Spec(join_usage) {
        const char*	strs[]	= {"a","b","c","d"};

        AssertThat(string_join(strs, strs + sizeof(strs)/sizeof(strs[0]), "|"),	Equals("a|b|c|d"));
        AssertThat(string_join(strs, strs + sizeof(strs)/sizeof(strs[0]), ",", "{", "}"),	Equals("{a,b,c,d}"));
    }

    Spec(split_usage) {
        std::deque<std::string>	q;

        q.clear();
        string_split("a, b, c|d", ",|", std::back_inserter(q), true);
        AssertThat(q,		HasLength(4));
        AssertThat(q[0],	Equals("a"));
        AssertThat(q[1],	Equals(" b"));
        AssertThat(q[2],	Equals(" c"));
        AssertThat(q[3],	Equals("d"));

        q.clear();
        string_split("a,,b c|d", ",|", std::back_inserter(q), true);
        AssertThat(q,	HasLength(3));
        AssertThat(q[0],	Equals("a"));
        AssertThat(q[1],	Equals("b c"));
        AssertThat(q[2],	Equals("d"));

        q.clear();
        string_split("a,,b c|d", ",|", std::back_inserter(q), false);
        AssertThat(q,	HasLength(4));
        AssertThat(q[0],	Equals("a"));
        AssertThat(q[1],	Equals(""));
        AssertThat(q[2],	Equals("b c"));
        AssertThat(q[3],	Equals("d"));
    }

    Spec(split_usage2) {
        std::deque<std::string>	q;

        q.clear();
        string_split("1, 20, 300, 4000", ",\t |", std::back_inserter(q), true);
        AssertThat(q,		HasLength(4));
        AssertThat(q[0],	Equals("1"));
        AssertThat(q[1],	Equals("20"));
        AssertThat(q[2],	Equals("300"));
        AssertThat(q[3],	Equals("4000"));
    }

    Spec(match_wildcard_usage) {
        AssertThat(string_match_wildcard("XiMenPo", "Men*"),	IsFalse());
        AssertThat(string_match_wildcard("XiMenPo", "XiMen*"),	IsTrue());
        AssertThat(string_match_wildcard("XiMenPo", "Xi?en"),	IsFalse());
        AssertThat(string_match_wildcard("XiMenPo", "X?Men*"),	IsTrue());
    }

    Spec(isnumber_usage) {
        AssertThat(string_isnumber(""),			IsFalse());
        AssertThat(string_isnumber(" "),		IsFalse());
        AssertThat(string_isnumber(" 0 "),		IsFalse());
        AssertThat(string_isnumber(" 0"),		IsTrue());
        AssertThat(string_isnumber(" 0.123"),	IsTrue());
        AssertThat(string_isnumber("-123.456"),	IsTrue());
        AssertThat(string_isnumber("-123.456e8"),	IsTrue());
        AssertThat(string_isnumber("-123.456e+0.8"),IsFalse());
        AssertThat(string_isnumber("-123.456e+0"),	IsTrue());
        AssertThat(string_isnumber("-123.456e+1"),	IsTrue());
        AssertThat(string_isnumber("-123.456e-8"),	IsTrue());
        AssertThat(string_isnumber("-123.456ee-8"),	IsFalse());
    }

    Spec(tobool_no_usage) {
        bool	b	= false;

        AssertThat(string_tobool(" 0 ", b),	IsTrue());
        AssertThat(b,	IsFalse());

        AssertThat(string_tobool(" F ", b),	IsTrue());
        AssertThat(b,	IsFalse());

        AssertThat(string_tobool(" N ", b),	IsTrue());
        AssertThat(b,	IsFalse());

        AssertThat(string_tobool(" OFF ", b),	IsTrue());
        AssertThat(b,	IsFalse());

        AssertThat(string_tobool(" FALSE ", b),	IsTrue());
        AssertThat(b,	IsFalse());

        AssertThat(string_tobool(" no ", b),	IsTrue());
        AssertThat(b,	IsFalse());
    }

    Spec(tobool_yes_usage) {
        bool	b	= false;
        AssertThat(string_tobool(" 1 ", b),	IsTrue());
        AssertThat(b,	IsTrue());

        AssertThat(string_tobool(" t ", b),	IsTrue());
        AssertThat(b,	IsTrue());

        AssertThat(string_tobool(" y ", b),	IsTrue());
        AssertThat(b,	IsTrue());

        AssertThat(string_tobool(" On ", b),	IsTrue());
        AssertThat(b,	IsTrue());

        AssertThat(string_tobool(" true ", b),	IsTrue());
        AssertThat(b,	IsTrue());

        AssertThat(string_tobool(" yes ", b),	IsTrue());
        AssertThat(b,	IsTrue());
    }

    Spec(tobool_failed_usage) {
        bool	b	= false;
        AssertThat(string_tobool(" 10 ", b),	IsFalse());
        AssertThat(string_tobool(" tru ", b),	IsFalse());
        AssertThat(string_tobool(" yesno ", b),	IsFalse());
        AssertThat(string_tobool(" noyes ", b),	IsFalse());
    }

    Spec(tonumbers_int_usage) {
        int	n1, n2, n3, n4;

        AssertThat(string_tonumbers("1, 20, 300, 4000", n1),	IsTrue());
        AssertThat(n1,	Equals(1));

        AssertThat(string_tonumbers("1, 20, 300, 4000", n1, n2),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));

        AssertThat(string_tonumbers("1, 20, 300, 4000", n1, n2, n3),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));

        AssertThat(string_tonumbers("1, 20, 300, 4000", n1, n2, n3, n4),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));
        AssertThat(n4,	Equals(4000));
    }

    Spec(tonumbers_int_usage2) {
        int	n1, n2, n3, n4;

        AssertThat(string_tonumbers("1 20 300 4000", n1),	IsTrue());
        AssertThat(n1,	Equals(1));

        AssertThat(string_tonumbers("1 20 300 4000", n1, n2),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));

        AssertThat(string_tonumbers("1 20 300 4000", n1, n2, n3),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));

        AssertThat(string_tonumbers("1 20 300 4000", n1, n2, n3, n4),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));
        AssertThat(n4,	Equals(4000));
    }

    Spec(tonumbers_int_usage3) {
        int	n1, n2, n3, n4;

        AssertThat(string_tonumbers("1\t 20\t 300\t 4000", n1),	IsTrue());
        AssertThat(n1,	Equals(1));

        AssertThat(string_tonumbers("1\t 20\t 300\t 4000", n1, n2),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));

        AssertThat(string_tonumbers("1\t 20\t 300\t 4000", n1, n2, n3),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));

        AssertThat(string_tonumbers("1\t 20\t 300\t 4000", n1, n2, n3, n4),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));
        AssertThat(n4,	Equals(4000));
    }

    Spec(tonumbers_int_usage4) {
        int	n1, n2, n3, n4;

        AssertThat(string_tonumbers("1|20|300|4000", n1),	IsTrue());
        AssertThat(n1,	Equals(1));

        AssertThat(string_tonumbers("1|20|300|4000", n1, n2),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));

        AssertThat(string_tonumbers("1|20|300|4000", n1, n2, n3),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));

        AssertThat(string_tonumbers("1|20|300|4000", n1, n2, n3, n4),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(n3,	Equals(300));
        AssertThat(n4,	Equals(4000));
    }

    Spec(tonumbers_mixtype_usage) {
        int	n1, n2;
        double d3, d4;

        AssertThat(string_tonumbers("1, 20, 300, 4000", n1, n2, d3, d4),	IsTrue());
        AssertThat(n1,	Equals(1));
        AssertThat(n2,	Equals(20));
        AssertThat(d3,	EqualsWithDelta(300, 0.01));
        AssertThat(d4,	EqualsWithDelta(4000, 0.01));
    }

    Spec(replace_usage) {
        std::string		s1, s2;
        s1 = s2 = "XiMenPo is simple! XiMenPo is good!";
        string_replace(s1, "XiMenPo", "Tao");
        AssertThat(s1,												Equals("Tao is simple! Tao is good!"));
        string_replace_recursive(s2, "XiMenPo", "Tao");
        AssertThat(s2,												Equals("Tao is simple! Tao is good!"));
    }

    Spec(replace_recursive_difference_usage) {
        std::string		s1, s2;
        s1 = s2 = "XiMenPoPo is simple!";
        string_replace(s1, "XiMenPo", "XiMen");
        AssertThat(s1,												Equals("XiMenPo is simple!"));
        string_replace_recursive(s2, "XiMenPo", "XiMen");
        AssertThat(s2,												Equals("XiMen is simple!"));
    }

    Spec(replace_const_string_usage) {
        AssertThat(string_replace("XiMenPoPo is simple!", "XiMenPo", "XiMen"),	Equals("XiMenPo is simple!"));
        AssertThat(string_replace_recursive("XiMenPoPo is simple!", "XiMenPo", "XiMen"),	Equals("XiMen is simple!"));
    }

    Spec(ellipsis_usage) {
        AssertThat(string_ellipsis("XiMenPo", 10),					Equals("XiMenPo"));
        AssertThat(string_ellipsis("XiMenPo", 7),					Equals("XiMenPo"));
        AssertThat(string_ellipsis("XiMenPo", 6),					Equals("XiMe.."));
        AssertThat(string_ellipsis("XiMenPo", 5),					Equals("XiM.."));
    }

    Spec(html_strip_tag_usage) {
        std::string	s1("<b>XiMenPo</b>"), s2("<b>XiMenPo</b");

        AssertThat(string_html_strip_tag(s1),	Equals("XiMenPo"));
        AssertThat(s1,	Equals("XiMenPo"));
        AssertThat(string_html_strip_tag(s2),	Equals("XiMenPo"));
        AssertThat(s2,	Equals("XiMenPo"));
    }

    Spec(ifind_usage) {
        AssertThat(string_ifind("XiMenPo is simple!", "ximenpo"),	Equals("XiMenPo is simple!"));
        AssertThat(string_ifind("XiMenPo is simple!", "SIMPLE"),	Equals("simple!"));
    }

    Spec(ifind_string_usage) {
        std::string	str("XiMenPo is simple!");
        AssertThat(string_ifind(str, "ximenpo"),	Equals(0));
        AssertThat(string_ifind(str, "SIMPLE"),		Equals(11));
        AssertThat(string_ifind(str, "IBM"),		Equals(std::string::npos));
    }

    Spec(ireplace_usage) {
        char	buf[32]	= {0};
        AssertThat(string_ireplace("|&lt;|", "&lt;", "<", buf) > 0, IsTrue());
        AssertThat(buf,	Equals("|<|"));
    }

    Spec(ireplace_string_usage) {
        std::string	str("XiMenPo is simple!");

        string_ireplace(str, "ximenpo", "Simple");
        AssertThat(str,	Equals("Simple is simple!"));
    }

    Spec(ireplace_string_usage2) {
        std::string	str("Simple is simple!");

        string_ireplace(str, "SIMPLE", "XiMenPo");
        AssertThat(str,	Equals("XiMenPo is XiMenPo!"));
    }

    Spec(html_to_text_usage) {
        AssertThat(string_html_to_text("|&gt;|"),	Equals("|>|"));
        AssertThat(string_html_to_text("|&lt;|"),	Equals("|<|"));
        AssertThat(string_html_to_text("&lt;<b>XiMenPo</b>&gt;&nbsp;is&nbsp;&amp;simple!"),	Equals("<XiMenPo> is &simple!"));
    }

    Spec(line_escape_usage) {
        std::string	s1, s2;
        s1	= "XiMenPo\\Simple is the same one?\nYes! it is!\n\rOK\r";
        s2	= "XiMenPo\\\\Simple is the same one?\\nYes! it is!\\n\\rOK\\r";
        AssertThat(string_line_escape(s1),	Equals(s2));
        AssertThat(string_line_unescape(s2),	Equals(s1));
    }

    Spec(string_iless_usage) {
        AssertThat(string_iless()("aBc", "acB"),		IsTrue());
        AssertThat(string_iless()("acB", "aBc"),		IsFalse());
        AssertThat(string_iless()("aBc", "aBc"),		IsFalse());
    }

    Spec(string_line_usage) {
        std::deque<string_line>    lines;
        std::string str("1 2\n3 4\n5 6\n");

        std::istringstream  is(str);
        std::ostringstream  os;
        std::copy(std::istream_iterator<string_line>(is),
                  std::istream_iterator<string_line>(),
                  std::back_inserter(lines)
                 );
        std::copy(lines.begin(), lines.end(), std::ostream_iterator<string_line>(os, "\n"));

        AssertThat(os.str(),	Equals(str));
    }

    Spec(json_escape_usage) {
        std::string	s1, s2;
        s1	= "XiMenPo\\Simple\tis the /same one?\nYes! it is!\n\rOK\r";
        s2	= "XiMenPo\\\\Simple\\tis the \\/same one?\\nYes! it is!\\n\\rOK\\r";
        AssertThat(string_json_escape(s1),	Equals(s2));
        AssertThat(string_json_unescape(s2),	Equals(s1));
    }

    Spec(xml_escape_usage) {
        std::string	s1, s2;
        s1	= "Is <XiMenPo> & Simple the 'same one'?\n\"Yes! it is!\"\n\rOK\r";
        s2	= "Is &lt;XiMenPo&gt; &amp; Simple the &apos;same one&apos;?\n&quot;Yes! it is!&quot;\n\rOK\r";
        AssertThat(string_xml_escape(s1),	Equals(s2));
        AssertThat(string_xml_unescape(s2),	Equals(s1));
    }
};
