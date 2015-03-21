#include <igloo/igloo.h>
using namespace igloo;

#include "simple/stringify.h"

Context(stringify_ini_context) {
    stringify_data	data;
    void	SetUp() {
        data.clear();
    }

    Spec(basic_usage) {
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
        AssertThat(stringify_to_ini_stream(data, os, "/"),	IsTrue());
        AssertThat(os.str(),	Equals(""
                                       "[]\n"
                                       "age=34\n"
                                       "name=XiMenPo\n"
                                       "[object]\n"
                                       "sex=1\n"
                                       "sname=XiMenPo\n"
                                       "[array]\n"
                                       "101\n"
                                       "102\n"
                                       "103\n"
                                       "[]\n"
                                       "another_data=another_data\n"
                                   ));
    }

    Spec(mixed_object_usage) {
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
        AssertThat(stringify_to_ini_stream(data, os, "/"),	IsTrue());
        AssertThat(os.str(),	Equals(""
                                       "[]\n"
                                       "age=34\n"
                                       "name=XiMenPo\n"
                                       "[object]\n"
                                       "sex=1\n"
                                       "[object/array]\n"
                                       "101\n"
                                       "102\n"
                                       "103\n"
                                       "[object]\n"
                                       "sname=XiMenPo\n"
                                       "[]\n"
                                       "another_data=another_data\n"
                                   ));
    }

    Spec(from_stream_usage) {
        std::string	ini	= ""
                          "[]\n"
                          "age=34\n"
                          "name=XiMenPo\n"
                          "[object]\n"
                          "sex=1\n"
                          "[object/array]\n"
                          "101\n"
                          "102\n"
                          "103\n"
                          "[object]\n"
                          "sname=XiMenPo\n"
                          "[]\n"
                          "another_data=another_data\n"
                          ;
        std::istringstream	is(ini);
        std::ostringstream	os;

        AssertThat(stringify_from_ini_stream(data, is, "/"),	IsTrue());
        AssertThat(stringify_to_ini_stream(data, os, "/"),		IsTrue());
        AssertThat(os.str(),	Equals(ini));
    }

    Spec(array_of_array_to_ini_test) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	int(34))
        .field("name",	"XiMenPo")
        .object_begin("object")
        .field("sex",	true)
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
        .field("sname",	std::string("XiMenPo"))
        .object_end()
        .field("another_data",	"another_data")
        .object_end()
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_ini_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "[]\n"
                       "age=34\n"
                       "name=XiMenPo\n"
                       "[object]\n"
                       "sex=1\n"
                       "[object/array]\n"
                       "[object/array/]\n"
                       "101\n"
                       "102\n"
                       "103\n"
                       "[object/array/]\n"
                       "101\n"
                       "102\n"
                       "103\n"
                       "[object/array/]\n"
                       "101\n"
                       "102\n"
                       "103\n"
                       "[object]\n"
                       "sname=XiMenPo\n"
                       "[]\n"
                       "another_data=another_data\n"
                   ));
    }

    Spec(non_leaf_array_from_not_support) {
        std::string	ini	= ""
                          "[]\n"
                          "age=34\n"
                          "name=XiMenPo\n"
                          "[object]\n"
                          "sex=1\n"
                          "[object/array]\n"
                          "[object/array/]\n"
                          "101\n"
                          "102\n"
                          "103\n"
                          "[object/array/]\n"
                          "101\n"
                          "102\n"
                          "103\n"
                          "[object/array/]\n"
                          "101\n"
                          "102\n"
                          "103\n"
                          "[object]\n"
                          "sname=XiMenPo\n"
                          "[]\n"
                          "another_data=another_data\n"
                          ;
        std::istringstream	is(ini);
        std::ostringstream	os;

        AssertThat(stringify_from_ini_stream(data, is, "/"),	IsFalse());
    }

};
