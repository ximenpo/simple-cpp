#include <simple/igloo.h>
using namespace igloo;

#include "simple/module.h"

//
//	dll as module.
//
class	testDummyModule : public module {
    DECLARE_SUPER_CLASS(module);
public:
    MODULE_DECLARE_FUNC(const char*, __cdecl, Call, (const char*))
public:
    MODULE_INIT_BEGIN()
    MODULE_INIT_FUNC(const char*, __cdecl, Call, (const char*), "dummy")
    MODULE_INIT_END();
};

//
//	test interface.
//
struct	testModuleIntf {
    virtual	const char*	do_somthing(const char*)	= 0;
};

//
//	interface module define.
//
class	testInterfaceModule : public interface_module<testModuleIntf> {
public:
    testInterfaceModule() : interface_module<testModuleIntf>("create_intf", "destroy_intf") {
    }
};

Context(module_context) {
    Spec(basic_usage) {
        testDummyModule	m;
        AssertThat(m.load("dummy.module"),	IsTrue());
        AssertThat(m.ready(),			IsTrue());
        AssertThat(m.Call,				!Equals((void*)0));
        AssertThat(m.Call("XiMenPo"),	Equals("XiMenPo"));
    }

    Spec(interface_module_usage) {
        testInterfaceModule	m;
        AssertThat(m.load("dummy.module"),	IsTrue());
        AssertThat(m.ready(),				IsTrue());
        if(m.ready()) {
            AssertThat(m->do_somthing("XiMenPo"),	Equals("XiMenPo"));
        }
    }
};
