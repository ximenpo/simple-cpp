#include <simple/igloo.h>
using namespace igloo;

#include <string>

#include <simple/property.h>

class	PropertyTest {
public:
    PropertyTest():action_(0) {}

    PROPERTY_SUPPORT(PropertyTest);
public:
    PROPERTY_SIMPLE(name, std::string);
    PROPERTY_SIMPLE(age, int);

    PROPERTY(action, int, getAction, setAction);
    PROPERTY_READONLY(readonly_action, int, getAction);
    PROPERTY_WRITEONLY(writeonly_action, int, setAction);

private:
    int		getAction() {
        return action_;
    }
    void	setAction(int n) {
        action_ = n;
    }

private:
    int		action_;
};

Context(property_usage) {
    Spec(simple_property_usage) {
        PropertyTest	obj;
        obj.name	= "ximenpo";
        obj.age		= 35;

        AssertThat(obj.name,	Equals("ximenpo"));
        AssertThat(obj.age,		Equals(35));
    }

    Spec(normal_property_usage) {
        PropertyTest	obj;

        AssertThat(obj.action == 0,		IsTrue());
        obj.action	= 35;
        AssertThat(obj.action == 35,	IsTrue());
    }

    Spec(readonly_property_usage) {
        PropertyTest	obj;

        AssertThat(obj.readonly_action == 0,	IsTrue());
        obj.action	= 35;
        AssertThat(obj.readonly_action == 35,	IsTrue());
    }

    Spec(writeonly_property_usage) {
        PropertyTest	obj;

        AssertThat(obj.action == 0,		IsTrue());
        obj.writeonly_action	= 35;
        AssertThat(obj.action == 35,	IsTrue());
    }
};
