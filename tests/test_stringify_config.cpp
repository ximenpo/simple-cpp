#include <simple/igloo.h>
using namespace igloo;

#include	"simple/string.h"
#include	"simple/stringify.h"

#include	<stdexcept>

Context(stringify_config_context) {
    stringify_data	data;

    void	SetUp() {
        data.clear();
    }

    Spec(has_and_get_usage) {
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
        .object_end()
        .end();

        stringify_config	cfg(&data);
        AssertThat(cfg.has("no"),					IsFalse());
        AssertThat(cfg.get("", "Nothing!"),			Equals("Nothing!"));
        AssertThat(cfg.has("age"),					IsTrue());
        AssertThat(cfg.get("age", 21),				Equals(34));
        AssertThat(cfg.has("object/sex"),			IsTrue());
        AssertThat(cfg.get("object/sex", false),	Equals(true));
        AssertThat(cfg.has("object/sname"),			IsTrue());
        AssertThat(cfg.get("object/sname", ""),		Equals("XiMenPo"));
        AssertThat(cfg.get("object", "sex", false),	Equals(true));
        AssertThat(cfg.get("object", "sname", ""),	Equals("XiMenPo"));
        AssertThat(cfg.has("array", 0),				IsTrue());
        AssertThat(cfg.has("array", 1),				IsTrue());
        AssertThat(cfg.has("array", 2),				IsTrue());
        AssertThat(cfg.has("array", 3),				IsFalse());
        AssertThat(cfg.get("array", 0, 0),			Equals(101));
        AssertThat(cfg.get("array", 1, 0),			Equals(102));
        AssertThat(cfg.get("array", 2, 0),			Equals(103));
        AssertThat(cfg.get("array", 3, 0),			Equals(0));
    }

    Spec(basic_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .object_begin("obj")
        .value(1, "v1")
        .value(2, "v2")
        .value(3, "v3")
        .value(4, "v4")

        .object_begin("type")
        .value(11,	"v1")
        .value(12,	"v2")
        .value(13, "v3")

        .object_end()

        .object_begin("group")
        .value(101,	"v1")
        .value(102,	"v2")
        .object_end()

        .object_begin("id")
        .value(1001,"v1")
        .object_end()

        .object_end()
        .object_end()
        .end();

        stringify_config	cfg(&data, "id", "group", "type");
        AssertThat(cfg.get("obj", "v1", 0),		Equals(1001));
        AssertThat(cfg.get("obj", "v2", 0),		Equals(102));
        AssertThat(cfg.get("obj", "v3", 0),		Equals(13));
        AssertThat(cfg.get("obj", "v4", 0),		Equals(4));
    }


    Spec(basic_root_variable_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .value(1, "v1")
        .value(2, "v2")
        .value(3, "v3")
        .value(4, "v4")

        .object_begin("type")
        .value(11,	"v1")
        .value(12,	"v2")
        .value(13,	"v3")
        .object_end()

        .object_begin("group")
        .value(101,	"v1")
        .value(102,	"v2")
        .object_end()

        .object_begin("id")
        .value(1001,"v1")
        .object_end()

        .object_end()
        .end();

        stringify_config	cfg(&data, "id", "group", "type");
        AssertThat(cfg.get("v1", 0),		Equals(1001));
        AssertThat(cfg.get("v2", 0),		Equals(102));
        AssertThat(cfg.get("v3", 0),		Equals(13));
        AssertThat(cfg.get("v4", 0),		Equals(4));
    }

    Spec(basic_root_section_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .value(1, "v1")
        .value(2, "v2")
        .value(3, "v3")
        .value(4, "v4")

        .object_begin("type")
        .value(11,	"v1")
        .value(12,	"v2")
        .value(13,	"v3")
        .object_end()

        .object_begin("group")
        .value(101,	"v1")
        .value(102,	"v2")
        .object_end()

        .object_begin("id")
        .value(1001,"v1")
        .object_end()

        .object_end()
        .end();

        stringify_config	cfg(&data, "id", "group", "type");
        AssertThat(cfg.has_section(""),		IsTrue());
        AssertThat(cfg.get_section(""),		Equals(data.get_container("id")));

        AssertThat(cfg.has_section("type"),		IsTrue());
        AssertThat(cfg.get_section("type"),		Equals(data.get_container("type")));
    }

    Spec(basic_section_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .object_begin("obj")
        .value(1, "v1")
        .value(2, "v2")
        .value(3, "v3")
        .value(4, "v4")

        .object_begin("type")
        .value(11,	"v1")
        .value(12,	"v2")
        .value(13,	"v3")
        .object_end()

        .object_begin("group")
        .value(101,	"v1")
        .value(102,	"v2")
        .object_end()

        .object_begin("id")
        .value(1001,"v1")
        .object_end()
        .object_end()

        .object_end()
        .end();

        stringify_config	cfg(&data, "id", "group", "type");
        AssertThat(cfg.has_section(""),		IsTrue());
        AssertThat(cfg.get_section(""),		Equals(data.get_container("")));

        AssertThat(cfg.has_section("obj"),		IsTrue());
        AssertThat(cfg.get_section("obj"),		Equals(data.get_container("obj/id")));
    }
};
