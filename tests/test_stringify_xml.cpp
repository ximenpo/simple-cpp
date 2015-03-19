#include <igloo/igloo.h>
using namespace igloo;

#include "simple/stringify.h"
#include "simple/stringify_xml.h"

Context(stringify_xml_context) {
    stringify_data	data;
    void	SetUp() {
        data.clear();
    }

    Spec(simple_value_to_xml_1) {
        stringify_data_builder(data).begin()
        .value(int(34))
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "<_>"
                       "34"
                       "</_>"
                   ));
    }

    Spec(simple_value_to_xml_2) {
        stringify_data_builder(data).begin()
        .value("XiMenPo")
        .end();

        std::ostringstream	os;
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "<_>"
                       "XiMenPo"
                       "</_>"
                   ));
    }

    Spec(to_xml_usage) {
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
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "<_>"
                       "<age>34</age>"
                       "<name>XiMenPo</name>"
                       "<object><sex>1</sex><sname>XiMenPo</sname></object>"
                       "<array><_>101</_><_>102</_><_>103</_></array>"
                       "<another_data>another_data</another_data>"
                       "</_>"
                   ));
    }

    Spec(array_of_array_to_xml_usage) {
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
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(
                       "<_>"
                       "<age>34</age>"
                       "<name>XiMenPo</name>"
                       "<object><sex>1</sex><sname>XiMenPo</sname></object>"
                       "<array>"
                       "<_><_>101</_><_>102</_><_>103</_></_>"
                       "<_><_>101</_><_>102</_><_>103</_></_>"
                       "<_><_>101</_><_>102</_><_>103</_></_>"
                       "</array>"
                       "<another_data>another_data</another_data>"
                       "</_>"
                   ));
    }

    Spec(from_xml_path_delimeter_test) {
        std::string	xml	=
            "<data>"
            "<age>34</age>"
            "<name>XiMenPo</name>"
            "</data>"
            ;
        {
            std::istringstream	is(xml);
            AssertThat(stringify_from_xml_stream(data, is),	IsFalse());
        }
        {
            std::istringstream	is(xml);
            AssertThat(stringify_from_xml_stream(data, is, "data"),	IsTrue());
        }
    }

    Spec(from_xml_usage) {
        std::string	xml	=
            "<_>"
            "<age>34</age>"
            "<name>XiMenPo</name>"
            "<object><sex>1</sex><sname>XiMenPo</sname></object>"
            "<array>"
            "<_><_>101</_><_>102</_><_>103</_></_>"
            "<_><_>101</_><_>102</_><_>103</_></_>"
            "<_><_>101</_><_>102</_><_>103</_></_>"
            "</array>"
            "<another_data>another_data</another_data>"
            "</_>"
            ;

        std::istringstream	is(xml);
        std::ostringstream	os;
        AssertThat(stringify_from_xml_stream(data, is),	IsTrue());
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(xml));
    }

    Spec(from_xml_with_empty_content) {
        std::string	xml	=
            "<_>"
            "<age>34</age>"
            "<name>XiMenPo</name>"
            "<empty></empty>"
            "<object></object>"
            "</_>"
            ;

        std::istringstream	is(xml);
        std::ostringstream	os;
        AssertThat(stringify_from_xml_stream(data, is),	IsTrue());
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(xml));
    }

    Spec(from_xml_null) {
        std::string	xml	=
            "<_>"
            "</_>"
            ;

        std::istringstream	is(xml);
        std::ostringstream	os;
        AssertThat(stringify_from_xml_stream(data, is),	IsTrue());
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(xml));
    }

    Spec(from_xml_simple_value) {
        std::string	xml	=
            "<_>"
            "34"
            "</_>"
            ;

        std::istringstream	is(xml);
        std::ostringstream	os;
        AssertThat(stringify_from_xml_stream(data, is),	IsTrue());
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(xml));
    }

    Spec(from_xml_simple_array) {
        std::string	xml	=
            "<_>"
            "<_>34</_>"
            "<_>35</_>"
            "<_>36</_>"
            "</_>"
            ;

        std::istringstream	is(xml);
        std::ostringstream	os;
        AssertThat(stringify_from_xml_stream(data, is),	IsTrue());
        AssertThat(stringify_to_xml_stream(data, os),	IsTrue());
        AssertThat(os.str(),	Equals(xml));
    }

};
