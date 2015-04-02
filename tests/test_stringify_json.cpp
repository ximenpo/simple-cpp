#include "simple/igloo.h"
using namespace igloo;

#include "simple/stringify.h"

Context(stringify_json_context) {
    stringify_data	data;
    void	SetUp() {
        data.clear();
    }

    Spec(simple_value_to_json_1) {
        stringify_data_builder(data).begin()
        .value(int(34))
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "34"
                   ));
    }

    Spec(simple_value_to_json_2) {
        stringify_data_builder(data).begin()
        .value("XiMenPo")
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "\"XiMenPo\""
                   ));
    }

    Spec(to_json_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	int(34))
        .field("name",	"XiMenPo")
        .object_begin("object")
        .field("sex",	true)
        .field("sname",	std::string("XiMenPo"))
        .object_end()
        .array_begin("array")
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .field("another_data",	"another_data")
        .object_end()
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals("{\"age\":34,\"name\":\"XiMenPo\",\""
                                       "object\":{\"sex\":1,\"sname\":\"XiMenPo\"},"
                                       "\"array\":[101,102,103],\"another_data\":\"another_data\"}"
                                   ));
    }

    Spec(to_json_mixed_object_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	int(34))
        .field("name",	"XiMenPo")
        .object_begin("object")
        .field("sex",	true)
        .array_begin("array")
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .field("sname",	std::string("XiMenPo"))
        .object_end()
        .field("another_data",	"another_data")
        .object_end()
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals("{\"age\":34,\"name\":\"XiMenPo\","
                                       "\"object\":{\"sex\":1,\"array\":[101,102,103],\"sname\":\"XiMenPo\"},"
                                       "\"another_data\":\"another_data\"}"
                                   ));
    }

    Spec(array_of_array_to_json_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	int(34))
        .field("name",	"XiMenPo")
        .object_begin("object")
        .field("sex",	true)
        .field("sname",	std::string("XiMenPo"))
        .object_end()
        .array_begin("array")
        .array_begin()
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .array_begin()
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .array_begin()
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .array_end()
        .field("another_data",	"another_data")
        .object_end()
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "{"
                       "\"age\":34,"
                       "\"name\":\"XiMenPo\","
                       "\"object\":{\"sex\":1,\"sname\":\"XiMenPo\"},"
                       "\"array\":["
                       "[101,102,103],"
                       "[101,102,103],"
                       "[101,102,103]"
                       "],"
                       "\"another_data\":\"another_data\""
                       "}"
                   ));
    }

    Spec(from_stream_usage) {
        // sorted members.
        std::string	json	= "{\"age\":34,\"another_data\":\"another_data\","
                              "\"name\":\"XiMenPo\","
                              "\"object\":{\"array\":[101,102,103],\"sex\":1,\"sname\":\"XiMenPo\"}"
                              "}"
                              ;
        std::istringstream	is(json);
        std::ostringstream	os;

        AssertThat(stringify_from_json_stream(data, is),	IsTrue());
        AssertThat(stringify_to_json_stream(data, os),		IsTrue());
        AssertThat(os.str(),	Equals(json));
    }

    Spec(from_stream_array_of_array) {
        // sorted members.
        std::string	json	= "{\"age\":34,\"another_data\":\"another_data\","
                              "\"name\":\"XiMenPo\","
                              "\"object\":{\"array\":[[101,102,103],[101,102,103],[101,102,103]],\"sex\":1,\"sname\":\"XiMenPo\"}"
                              "}"
                              ;
        std::istringstream	is(json);
        std::ostringstream	os;

        AssertThat(stringify_from_json_stream(data, is),	IsTrue());
        AssertThat(stringify_to_json_stream(data, os),		IsTrue());
        AssertThat(os.str(),	Equals(json));
    }

    Spec(from_json_empty_object) {
        std::string	json	=
            "{"
            "}"
            ;

        std::istringstream	is(json);
        std::ostringstream	os;
        AssertThat(stringify_from_json_stream(data, is),	IsTrue());
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(json));
    }

    Spec(from_json_empty_array) {
        std::string	json	=
            "["
            "]"
            ;

        std::istringstream	is(json);
        std::ostringstream	os;
        AssertThat(stringify_from_json_stream(data, is),	IsTrue());
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(json));
    }

    Spec(faile_from_json_null) {
        std::string	json	=
            "null"
            ;

        std::istringstream	is(json);
        std::ostringstream	os;
        AssertThat(stringify_from_json_stream(data, is),	IsFalse());
    }

    Spec(faile_from_json_simple_value) {
        std::string	json	=
            "34"
            ;

        std::istringstream	is(json);
        std::ostringstream	os;
        AssertThat(stringify_from_json_stream(data, is),	IsFalse());
    }

    Spec(from_json_simple_array) {
        std::string	json	=
            "[34,35,36]"
            ;

        std::istringstream	is(json);
        std::ostringstream	os;
        AssertThat(stringify_from_json_stream(data, is),	IsTrue());
        AssertThat(stringify_to_json_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(json));
    }

};
