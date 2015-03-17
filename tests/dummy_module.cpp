
#if	defined(_MSC_VER)
#	define	EXPORT	__declspec(dllexport)
#else
#	define	EXPORT
#endif

#if	!defined(INTERFACE_MODULE_CALLTYPE)
#	define	INTERFACE_MODULE_CALLTYPE
#endif

extern "C"
{
    EXPORT	const char* dummy(const char* sName);
}

const char*	dummy(const char* sName) {
    return sName;
}

//
//	test interface.
//
struct	testModuleIntf {
    virtual	const char*	do_somthing(const char*)	= 0;
};

struct	testModuleIntfImpl : public testModuleIntf {
    virtual	const char*	do_somthing(const char* input) {
        return	input;
    }
};

extern "C"
{
    EXPORT	testModuleIntf*		INTERFACE_MODULE_CALLTYPE	create_intf();
    EXPORT	void				INTERFACE_MODULE_CALLTYPE	destroy_intf(testModuleIntf* intf);
}

testModuleIntf*		INTERFACE_MODULE_CALLTYPE	create_intf() {
    return	new testModuleIntfImpl();
}

void				INTERFACE_MODULE_CALLTYPE	destroy_intf(testModuleIntf* intf) {
    delete	static_cast<testModuleIntfImpl*>(intf);
}
