#ifndef MODULE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define MODULE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	"delegate.h"
#include	"super.h"

//
//	DLL模块接口
//
class module {
public:
    typedef void*		MHANDLE;
public:
    ///	constructions.
    module();
    virtual	~module();

public:
    ///	operations.
    bool	load(const char* path);
    bool	ready();
    void	unload();

public:
    ///	operation handler.
    delegate<MHANDLE(const char* path)>						handler_load_library;
    delegate<void*(MHANDLE handle, const char* func_name)>	handler_get_proc_address;
    delegate<void(MHANDLE handle)>							handler_unload_library;

protected:
    ///	overridable operations.
    virtual	bool	do_get_ready();
    virtual	bool	do_load_library(const char* path);
    virtual	void*	do_get_proc_address(const char* func_name);
    virtual	void	do_unload_library();
    virtual	bool	do_load_proc_items();
    virtual	void	do_unload_proc_items();

protected:
    MHANDLE	handle_;

private:
    module(const module&);
    const module&	operator =(const module&);
};

//
//	macro defines.
//
#define MODULE_DECLARE_FUNC(RetType, CallType, CallName, Args)							\
	RetType	(CallType* CallName)Args;

#define MODULE_INIT_BEGIN()																\
	virtual bool do_load_proc_items(){													\
		bool	all_loaded = true;

#define MODULE_INIT_FUNC(RetType, CallType, CallName, Args, FuncName)					\
		this->CallName = (RetType(CallType*)Args)this->do_get_proc_address(FuncName);	\
		if (all_loaded && (0 == this->CallName)){										\
			all_loaded = false;															\
		}

#define MODULE_INIT_END() 																\
		return __super::do_load_proc_items() && all_loaded;								\
	}

//
//	macro usage:
//
#if		0
class	MyModule : public module {
public:
    MODULE_DECLARE_FUNC(void*, __stdcall, Call, (void))
public:
    MODULE_INIT_BEGIN()
    MODULE_INIT_FUNC(void*, __stdcall, Call, (void), "FuncName")
    MODULE_INIT_END()
};
#endif	//~0

#if	!defined(INTERFACE_MODULE_CALLTYPE)
#	define	INTERFACE_MODULE_CALLTYPE
#endif

//
//	简单DLL接口模块
//
//	Note:	关于函数调用方式，用默认方式跨平台最好【__cdecl方式】，
//			采用__stdcall时，gcc和msvc默认生成的dll导出函数是编码过的，比如：_create_intf@4
//
template<typename TheInterface>
class	interface_module :	public module {
    DECLARE_SUPER_CLASS(module);
public:
    interface_module( const char* create_name, const char* destroy_name )
        :	interface_(0)
        ,	create_func_name_(create_name)
        ,	destroy_func_name_(destroy_name) {
    }

    ~interface_module() {
        interface_module::unload();
    }

protected:
    virtual	bool		do_get_ready() {
        return	0 != interface_ && module::do_get_ready();
    }
    virtual	bool		do_load_proc_items() {
        bool	all_loaded = true;
        MODULE_INIT_FUNC(TheInterface*, INTERFACE_MODULE_CALLTYPE, create_func,		(),					create_func_name_);
        MODULE_INIT_FUNC(void,			INTERFACE_MODULE_CALLTYPE, destroy_func,	(TheInterface*),	destroy_func_name_);
        if(0 != handle_ && 0 != create_func) {
            interface_	= (*create_func)();
        }
        return	(0 != interface_ && 0 != create_func);
    }
    virtual	void		do_unload_proc_items() {
        if(0 != interface_ && 0 != destroy_func) {
            (*destroy_func)(interface_);
        }
        create_func	= 0;
        destroy_func= 0;
        interface_	= 0;
        __super::do_unload_proc_items();
    }

public:
    TheInterface*		operator->()const {
        return interface_;
    }
    operator			TheInterface*()const {
        return interface_;
    }

protected:
    TheInterface*		interface_;
    const char*			create_func_name_;
    const char*			destroy_func_name_;
    TheInterface*		(INTERFACE_MODULE_CALLTYPE	*create_func)	();
    void				(INTERFACE_MODULE_CALLTYPE	*destroy_func)	(TheInterface*);
};

//
//	interface_module usage:
//
#if	0
struct	MyIntf {
    virtual	void	do_somthing()	= 0;
};

class	MyModule : public interface_module<MyIntf> {
public:
    MyModule() : interface_module<MyIntf>("CreateIntf", "DestroyIntf") {
    }
};

void	test() {
    MyModule	m;
    if(m.load("dummy.module")) {
        m->do_somthing();
    }
}
#endif

#endif
