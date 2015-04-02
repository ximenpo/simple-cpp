#include "simple/igloo.h"
using namespace igloo;

#include "simple/enum.h"
#include "simple/binary.h"

enum	TestEnumType {
    TestEnum1,
    TestEnum2,
    TestEnum3,
    TestEnum4,
};

enum	TestEnumExType {
    TestEnumEx1,
    TestEnumEx2,
    TestEnumEx3,
    TestEnumEx4,
};

Context(enum_set_usage) {
    Spec(basic_usage) {
        enum_set<TestEnumType>	flags	= B01010;
        AssertThat(flags.is_set(TestEnum1),			IsFalse());
        AssertThat(flags.is_set(TestEnum2),			IsTrue());
        AssertThat(flags.is_set(TestEnum3),			IsFalse());
        AssertThat(flags.is_set(TestEnum4),			IsTrue());

        unsigned long	data	= flags;
        AssertThat(data,			Equals(flags.data()));

        flags.set(TestEnum3);
        flags.unset(TestEnum4);
        AssertThat(flags.is_set(TestEnum3),			IsTrue());
        AssertThat(flags.is_set(TestEnum4),			IsFalse());
    }
};

SET_ENUM_STRINGS(TestEnumType,
                 ENUM_NAME(TestEnum1)
                 ENUM_NAME(TestEnum2)
                 ENUM_NAME(TestEnum3)
                 ENUM_ITEM("enum4")
                );

SET_ENUM_STRINGS_EX(TestEnumExType,
                    ENUM_NAME_EX(TestEnumEx1)
                    //ENUM_NAME_EX(TestEnumEx2)
                    ENUM_ITEM_EX(TestEnumEx3, "enum3")
                    ENUM_NAME_EX(TestEnumEx4)
                   );

Context(enum_to_string_usage) {
    Spec(SET_ENUM_STRINGS_usage) {
        AssertThat(enum_to_string(TestEnum1),		Equals("TestEnum1"));
        AssertThat(enum_to_string(TestEnum2),		Equals("TestEnum2"));
        AssertThat(enum_to_string(TestEnum3),		Equals("TestEnum3"));
        AssertThat(enum_to_string(TestEnum4),		Equals("enum4"));
    }

    Spec(SET_ENUM_STRINGS_EX_usage) {
        AssertThat(enum_to_string(TestEnumEx1),		Equals("TestEnumEx1"));
        AssertThat(enum_to_string(TestEnumEx2),		Equals(""));
        AssertThat(enum_to_string(TestEnumEx3),		Equals("enum3"));
        AssertThat(enum_to_string(TestEnumEx4),		Equals("TestEnumEx4"));
    }
};
