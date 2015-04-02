#include "simple/igloo.h"
using namespace igloo;

//
//	通过定义IMPROVED_ENUM_NAME和IMPROVED_ENUM_LIST来定义一个枚举类型：Day
//
#define IMPROVED_ENUM_NAME  Day
#define IMPROVED_ENUM_LIST			\
	IMPROVED_ENUM_ITEM(Sunday)		\
	IMPROVED_ENUM_ITEM(Monday)		\
	IMPROVED_ENUM_ITEM(Tuesday)		\
	IMPROVED_ENUM_ITEM(Wednesday)	\
	IMPROVED_ENUM_ITEM(Thursday)	\
	IMPROVED_ENUM_ITEM(Friday)		\
	IMPROVED_ENUM_ITEM(Saturday)
#include "simple/define_improved_enum.inc"

Context(improved_enum_usage) {
    //
    //	在类中定义枚举类型：Color
    //
#define IMPROVED_ENUM_SUBCLASS
#define IMPROVED_ENUM_NAME  Color
#define IMPROVED_ENUM_LIST				\
		IMPROVED_ENUM_ITEM(Red)			\
		IMPROVED_ENUM_ITEM(Green)		\
		IMPROVED_ENUM_ITEM(Blue)
#include "simple/define_improved_enum.inc"

#define IMPROVED_ENUM_SUBCLASS
#define IMPROVED_ENUM_NAME  Company
#define IMPROVED_ENUM_FILE				"test_data/enum_company.inc"
#include "simple/define_improved_enum.inc"

#define IMPROVED_ENUM_SUBCLASS
#define IMPROVED_ENUM_NAME  MyCompany
#define IMPROVED_ENUM_LIST				\
		IMPROVED_ENUM_ITEM(XiMenPo)
#define IMPROVED_ENUM_INHERITED_NAME  Company
#define IMPROVED_ENUM_INHERITED_FILE	"test_data/enum_company.inc"
#include "simple/define_improved_enum.inc"

    void	CallWithCompany(Company::enum_type) {
    }

    void	CallWithMyCompany(MyCompany::enum_type) {
    }

    Spec(basic_usage) {
        Day::enum_type t = Day::Monday;
        AssertThat(Day::enum_to_string(t),			Equals("Monday"));
        AssertThat(Day::enum_from_string("Friday"),	Equals(Day::Friday));

        // Iteration:
        t = Day::enum_first_item();
        AssertThat(t,	Equals(Day::Sunday));
        t = Day::enum_next_item(t);
        AssertThat(t,	Equals(Day::Monday));
        t = Day::enum_last_item();
        AssertThat(t,	Equals(Day::Saturday));
        t = Day::enum_previous_item(t);
        AssertThat(t,	Equals(Day::Friday));

        // Count:
        AssertThat(Day::enum_valid_item_sum(), Equals(7));
    }

    Spec(in_class_usage) {
        Color::enum_type t = Color::Red;
        AssertThat(Color::enum_to_string(t),		Equals("Red"));
        AssertThat(Color::enum_from_string("Green"), Equals(Color::Green));

        // Iteration:
        t = Color::enum_first_item();
        AssertThat(t, Equals(Color::Red));
        t = Color::enum_next_item(t);
        AssertThat(t, Equals(Color::Green));
        t = Color::enum_last_item();
        AssertThat(t, Equals(Color::Blue));
        t = Color::enum_previous_item(t);
        AssertThat(t, Equals(Color::Green));

        // Count:
        AssertThat(Color::enum_valid_item_sum(), Equals(3));
    }

    Spec(other_function_usage) {
        AssertThat(Day::enum_separator(), Equals("::"));
        AssertThat(Day::enum_type_name(), Equals("Day"));
        AssertThat(Day::enum_parent_type_name(), Equals(""));
        AssertThat(Day::enum_full_type_name(), Equals("Day"));
        AssertThat(Day::enum_to_full_string(Day::Saturday), Equals("Day::Saturday"));
        AssertThat(Day::enum_from_full_string("Day::Saturday"), Equals(Day::Saturday));
    }

    Spec(extended_enum_usage) {
        Company::enum_type	c, cx;
        MyCompany::enum_type	mc, mcx, nmc;

        // Direct assignments:
        c	= Company::IBM; // OK
        mc	= MyCompany::XiMenPo; // OK
        AssertThat(c, Equals(Company::IBM));
        AssertThat(mc, Equals(MyCompany::XiMenPo));

        // Conversions to extended enum:
        mcx = MyCompany::enum_from_inherited(c); // OK
        AssertThat(mcx, Equals(MyCompany::IBM));

        mcx = MyCompany::enum_from_inherited(mc); // OK
        AssertThat(mcx, Equals(MyCompany::XiMenPo));

        // Conversions to base enum:
        c	= Company::IBM;
        cx	= MyCompany::enum_to_inherited(c); // OK
        AssertThat(cx, Equals(Company::IBM));
        cx	= MyCompany::enum_to_inherited(MyCompany::IBM); // OK
        AssertThat(cx, Equals(Company::IBM));
        cx	= MyCompany::enum_to_inherited(MyCompany::XiMenPo); // returns NotValidEnumItem
        AssertThat(cx, Equals(Company::enum_item_invalid));

        // The following compiles:
        CallWithCompany(Company::IBM); // OK
        CallWithCompany(MyCompany::enum_to_inherited(MyCompany::IBM)); // Possible NotValidEnumItem
        CallWithMyCompany(mc); // OK
        CallWithMyCompany(MyCompany::enum_from_inherited(Company::IBM)); // OK

        // Partial iteration:
        AssertThat(MyCompany::enum_first_extended_item(),					Equals(MyCompany::XiMenPo));;
        AssertThat(MyCompany::enum_next_inherited_item(MyCompany::Baidu),	Equals(MyCompany::enum_item_invalid));;
        AssertThat(MyCompany::enum_last_inherited_item(),					Equals(MyCompany::Baidu));;
        AssertThat(MyCompany::enum_previous_extended_item(MyCompany::enum_item_invalid),		Equals(MyCompany::XiMenPo));;

        // Partial count:
        AssertThat(MyCompany::enum_valid_inherited_item_sum(),	Equals(6));
        AssertThat(MyCompany::enum_valid_extended_item_sum(),	Equals(1));
    }

};
