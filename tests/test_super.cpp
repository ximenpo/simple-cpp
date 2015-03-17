#include <igloo/igloo.h>
using namespace igloo;

#include "simple/super.h"

//	super class
class	testSuperClass {
public:
    std::string	Method() {
        return	"testSuperClass";
    }
};

//	sub class, can use msvc __super keyword now.
class	testSubClass : public testSuperClass {
public:
    DECLARE_SUPER_CLASS(testSuperClass);
public:
    std::string	Method() {
        return	__super::Method();
    }
};

Context(super_context) {
    Spec(basic_usage) {
        testSubClass	cls;
        AssertThat(cls.Method(),	Equals("testSuperClass"));
    }
};
