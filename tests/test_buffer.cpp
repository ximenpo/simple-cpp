#include "simple/igloo.h"
using namespace igloo;

#include	<map>
#include	<set>
#include	<list>
#include	<deque>
#include	<vector>

#include	"simple/buffer.h"

struct testBufferMsg1 {
    enum			{ID	= 1};

    int				n;
    std::string		s;
};
buffer& operator>>(buffer& buf, testBufferMsg1& obj) {
    uintmax_t	size;
    uintmax_t	ver	= 0;
    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_OBJECT
            ||	!buffer_read_uint_value(buf, tag.size_tag, size)
            ||	(tag.version_tag && !buffer_read_uint_value(buf, buffer_tag::TAG_1, ver))
            //||	(tag.version_tag && ver>=0 && size < 0)
            ||	(tag.version_tag && ver<=0 && size > 0)
      ) {
        buf.set_failure();
        return	buf;
    }
    if(!tag.version_tag || ver == 0) {
        return	buf
                >>	obj.n
                >>	obj.s
                ;
    }
    size_t	count	= 0;
    for(size_t i = count; count < size; ++i) {
        buffer_read_and_ignore(buf);// ignore extended fields
    }
    return	buf;
}

buffer& operator<<(buffer& buf, const testBufferMsg1& obj) {
    uintmax_t  	size	= 0;
    buffer_tag	tag	= {
        buffer_tag::TYPE_OBJECT,
        buffer_size_tag(size),
        0 != 0,
    };
    if(		!buffer_write_tag(buf, tag)
            ||	!buffer_write_uint_value(buf, tag.size_tag, size)
            ||	(tag.version_tag && !buffer_write_uint_value(buf, buffer_tag::TAG_1, 0))
      ) {
        buf.set_failure();
        return	buf;
    }
    buf
            <<	obj.n
            <<	obj.s
            ;
    return	buf;
}

Context(buffer_context) {
    Spec(simple_msg_usage) {
        testBufferMsg1	m1	= {	35,
                                "XiMenPo",
                            };
        testBufferMsg1	m2;

        buffer	buf;
        buf	<<	m1;
        buf.rewind();
        buf	>>	m2;
        AssertThat(m2.n,	Equals(m1.n));
        AssertThat(m2.s,	Equals(m1.s));
    }

    Spec(map_usage) {
        std::map<int, std::string>	v1, v2;
        v1[0]	= "000";
        v1[1]	= "111";
        v1[2]	= "222";

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2.size(),	Equals(3));
        AssertThat(v2[0],		Equals(v1[0]));
        AssertThat(v2[1],		Equals(v1[1]));
        AssertThat(v2[2],		Equals(v1[2]));
    }

    Spec(deque_usage) {
        std::deque<std::string>	v1, v2;
        v1.push_back("000");
        v1.push_back("111");
        v1.push_back("222");

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2,	EqualsContainer(v1));
    }

    Spec(vector_usage) {
        std::vector<std::string>	v1, v2;
        v1.push_back("000");
        v1.push_back("111");
        v1.push_back("222");

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2,	EqualsContainer(v1));
    }

    Spec(list_usage) {
        std::list<std::string>	v1, v2;
        v1.push_back("000");
        v1.push_back("111");
        v1.push_back("222");

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2,	EqualsContainer(v1));
    }

    Spec(set_usage) {
        std::set<std::string>	v1, v2;
        v1.insert("000");
        v1.insert("111");
        v1.insert("222");

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2,	EqualsContainer(v1));
    }

    Spec(safe_array_usage) {
        safe_array<std::string, 3>	v1, v2;
        v1[0]	= "000";
        v1[1]	= "111";
        v1[2]	= "222";

        buffer	buf;
        buf	<<	v1;
        buf.rewind();
        buf	>>	v2;
        AssertThat(v2[0],		Equals(v1[0]));
        AssertThat(v2[1],		Equals(v1[1]));
        AssertThat(v2[2],		Equals(v1[2]));
    }

    Spec(queue_mergeable_usage) {
        buffer_queue	q(true);

        int	n;
        buffer	*r1, *r2, *r3, *r4, *r5;
        buffer	*b1 = q.create(), *b2 = q.create(), *b3 = q.create(), *b4 = q.create();
        (*b1)	<<	1;
        (*b2)	<<	2;
        (*b3)	<<	3;
        (*b4)	<<	4;

        AssertThat(q.get(r1),	Equals(false));

        q.push(b1);
        q.push(b2);
        q.push(b3);
        q.push(b4);

        AssertThat(q.get(r1),	Equals(true));
        (*r1) >> n;
        AssertThat(n,			Equals(1));

        AssertThat(q.get(r2),	Equals(true));
        (*r2) >> n;
        AssertThat(n,			Equals(2));

        AssertThat(q.get(r3),	Equals(true));
        (*r3) >> n;
        AssertThat(n,			Equals(3));

        AssertThat(q.get(r4),	Equals(true));
        (*r4) >> n;
        AssertThat(n,			Equals(4));

        AssertThat(q.get(r5),	Equals(false));

        AssertThat(r1,		Equals(r2));
        AssertThat(r2,		Equals(r3));
        AssertThat(r3,		Equals(r4));
    }

    Spec(queue_not_mergeable_usage) {
        buffer_queue	q(false);

        int	n;
        buffer	*r1, *r2, *r3, *r4, *r5;
        buffer	*b1 = q.create(), *b2 = q.create(), *b3 = q.create(), *b4 = q.create();
        (*b1)	<<	1;
        (*b2)	<<	2;
        (*b3)	<<	3;
        (*b4)	<<	4;

        AssertThat(q.get(r1),	Equals(false));

        q.push(b1);
        q.push(b2);
        q.push(b3);
        q.push(b4);

        AssertThat(q.get(r1),	Equals(true));
        (*r1) >> n;
        AssertThat(n,			Equals(1));

        AssertThat(q.get(r2),	Equals(true));
        (*r2) >> n;
        AssertThat(n,			Equals(2));

        AssertThat(q.get(r3),	Equals(true));
        (*r3) >> n;
        AssertThat(n,			Equals(3));

        AssertThat(q.get(r4),	Equals(true));
        (*r4) >> n;
        AssertThat(n,			Equals(4));

        AssertThat(q.get(r5),	Equals(false));

        AssertThat(r1,		!Equals(r2));
        AssertThat(r2,		!Equals(r3));
        AssertThat(r3,		!Equals(r4));
    }

    Spec(dump_usage) {
        buffer	buf;
        for(size_t i = 0; i < 32; ++i) {
            buf	<<	i;
        }

        std::ostringstream	os;
        buf.dump(os);
        AssertThat(os.str(),	EqualsContainer(buf.dump()));
    }

};
