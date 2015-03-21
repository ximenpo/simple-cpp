#include <igloo/igloo.h>
using namespace igloo;

#include "simple/stringify.h"

Context(stringify_lua_context) {
    stringify_data	data;
    void	SetUp() {
        data.clear();
    }

    Spec(simple_value_to_lua_1) {
        stringify_data_builder(data).begin()
        .value(int(34))
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_lua_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "34"
                   ));
    }

    Spec(simple_value_to_lua_2) {
        stringify_data_builder(data).begin()
        .value("XiMenPo")
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_lua_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "\"XiMenPo\""
                   ));
    }

    Spec(to_lua_usage) {
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
        AssertThat(stringify_to_lua_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals("{age=34,name=\"XiMenPo\","
                                       "object={sex=1,sname=\"XiMenPo\"},"
                                       "array={101,102,103},another_data=\"another_data\"}"
                                   ));
    }

    Spec(to_lua_mixed_object_usage) {
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
        AssertThat(stringify_to_lua_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals("{age=34,name=\"XiMenPo\","
                                       "object={sex=1,array={101,102,103},sname=\"XiMenPo\"},"
                                       "another_data=\"another_data\"}"
                                   ));
    }

    Spec(array_of_array_to_lua_usage) {
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
        AssertThat(stringify_to_lua_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "{"
                       "age=34,"
                       "name=\"XiMenPo\","
                       "object={sex=1,sname=\"XiMenPo\"},"
                       "array={"
                       "{101,102,103},"
                       "{101,102,103},"
                       "{101,102,103}"
                       "},"
                       "another_data=\"another_data\""
                       "}"
                   ));
    }

};
