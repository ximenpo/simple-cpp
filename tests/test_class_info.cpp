#include "simple/igloo.h"
using namespace igloo;

#include	<memory>
#include	"simple/class_info.h"

namespace class_info_test {

class	Widget;
typedef	class_info<Widget>	WidgetClassInfo;

class	Widget	: public support_root_class_info<Widget, WidgetClassInfo> {
public:
    virtual	const char*	Display() {
        return	"";
    }
};

class	Label	: public support_class_info<Label, Widget> {
public:
    virtual	const char*	Display() {
        return	"Label";
    }
};

class	Button	: public support_class_info<Button, Widget> {
public:
    virtual	const char*	Display() {
        return	"Button";
    }

public:
    static	const char*	get_static_class_name() {
        return	"Button";
    }
};

class	SimpleLabel	: public support_class_info<SimpleLabel, Label> {
};

class	RTFLabel	: public support_class_info<RTFLabel, Label> {
};

class	HTMLLabel	: public support_class_info<HTMLLabel, Label> {
};

class	DonotUseThisClass	: public support_class_info<DonotUseThisClass, Button> {
};

struct	PredCountSum {
    bool	operator()(const WidgetClassInfo* pInfo) {
        (*count)++;
        return	false;
    }
    int	*count;
};

//	! needed!!! initialize the class_info objects.
static	const WidgetClassInfo*	WidgetClassInfo_InitializeList[]	= {
    Label::get_static_class_info(),
    Button::get_static_class_info(),
    SimpleLabel::get_static_class_info(),
    RTFLabel::get_static_class_info(),
    HTMLLabel::get_static_class_info(),
    DonotUseThisClass::get_static_class_info(),
};

}

using	namespace	class_info_test;

Context(class_info_usage) {
    Spec(basic_usage) {
        std::auto_ptr<Widget>	obj(new Label);

        AssertThat(obj->get_class_info() != 0,				IsTrue());
        AssertThat(obj->get_class_info()->class_name,		Equals(typeid(Label).name()));
        AssertThat(obj->get_class_info()->instance_size,	Equals(sizeof(Label)));
        AssertThat((bool)obj->get_class_info()->create_instance,	IsTrue());
        AssertThat((bool)obj->get_class_info()->destroy_instance,	IsTrue());
    }

    Spec(custom_name_usage) {
        std::auto_ptr<Widget>	obj(new Button);

        AssertThat(obj->get_class_info(),				!Equals((void*)0));
        AssertThat(obj->get_class_info()->class_name,	Equals("Button"));
    }

    Spec(create_usage) {
        Widget*	pWidget	= Button::get_static_class_info()->create_instance();
        AssertThat(pWidget->Display(),	Equals("Button"));
        Button::get_static_class_info()->destroy_instance(pWidget);
    }

    Spec(class_info_tree_test) {
        const WidgetClassInfo*	pWidget	= Widget::get_static_class_info();
        const WidgetClassInfo*	pLabel	= Label::get_static_class_info();
        const WidgetClassInfo*	pButton	= Button::get_static_class_info();
        AssertThat(pWidget->base_class_info,	Equals((void*)0));
        AssertThat(pWidget->next_class_info,	Equals((void*)0));
        AssertThat(pWidget->prev_class_info,	Equals((void*)0));
        AssertThat(pWidget->sub_class_info,		!Equals((void*)0));
        AssertThat(pWidget->sub_class_info == pLabel || pWidget->sub_class_info == pButton,	IsTrue());

        AssertThat(pLabel->base_class_info,		Equals(pWidget));
        AssertThat(pButton->base_class_info,	Equals(pWidget));
    }

    Spec(more_level_class_test) {
        AssertThat(Label::get_static_class_info()->sub_class_info,			!Equals((void*)0));
        AssertThat(SimpleLabel::get_static_class_info()->base_class_info,	Equals(Label::get_static_class_info()));
        AssertThat(RTFLabel::get_static_class_info()->base_class_info,		Equals(Label::get_static_class_info()));
        AssertThat(HTMLLabel::get_static_class_info()->base_class_info,		Equals(Label::get_static_class_info()));
    }

    Spec(find_if_test) {
        int	count	= 0;
        PredCountSum	p	= {&count};
        AssertThat(Widget::get_static_class_info()->accept(p),		Equals((void*)0));
        AssertThat(count,	Equals(7));
    }

    Spec(find_test) {
        const WidgetClassInfo*	pInfo	= Label::get_static_class_info();
        AssertThat(pInfo->find("Button"),		Equals(Button::get_static_class_info()));
        AssertThat(pInfo->find("NotExist"),		Equals((void*)0));
    }
};
