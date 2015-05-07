
#include	"simple/module.h"

///	handle define.
#ifdef	_WIN32
#include	<windows.h>
#	define	S_LoadLibrary(path)						LoadLibrary(path)
#	define	S_GetProcAddress(handle, func_name)		GetProcAddress((HMODULE)handle, func_name)
#	define	S_FreeLibrary(handle)					FreeLibrary((HMODULE)handle)
#else
#	include	<dlfcn.h>
#	define	S_LoadLibrary(path)						dlopen(path,RTLD_LAZY)
#	define	S_GetProcAddress(handle, func_name)		dlsym(handle, func_name)
#	define	S_FreeLibrary(handle)					dlclose(handle)
#endif


module::module()
    : handle_(0) {
}

module::~module() {
    assert(!module::do_get_ready());
    if(module::do_get_ready()) module::do_unload_library();
}

bool module::ready() {
    return	do_get_ready();
}

bool module::load(const char* path) {
    if(ready()) unload();
    if(!do_load_library(path)) return false;
    do_load_proc_items();
    if(!ready()) unload();
    return ready();
}

void module::unload() {
    if(!ready()) return;
    do_unload_proc_items();
    do_unload_library();
}

bool module::do_get_ready() {
    return handle_ != 0;
}

bool module::do_load_library(const char* path) {
    assert(0 == handle_);
    handle_ = handler_load_library ? handler_load_library(path) : S_LoadLibrary(path);
    return handle_ != 0;
}

void* module::do_get_proc_address(const char* func_name) {
    assert(0 != handle_);
    if(handler_get_proc_address) {
        return	handler_get_proc_address(handle_, func_name);
    } else {
        return	S_GetProcAddress(handle_, func_name);
    }
}

void module::do_unload_library() {
    if(0 != handle_) {
        if(handler_unload_library) {
            handler_unload_library(handle_);
        } else {
            S_FreeLibrary(handle_);
        }
    }
    handle_ = 0;
}

bool module::do_load_proc_items() {
    return true;
}

void module::do_unload_proc_items() {
}
