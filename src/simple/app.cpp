
#include	<string>

#if	defined(_MSC_VER)

#include	<cstdlib>
extern	"C"	unsigned long	__stdcall	GetModuleFileNameA(void* hModule, const char* lpFilename, unsigned long nSize);
std::string	app_dir() {
    char buf[1024]	= {0};
    GetModuleFileNameA(0, buf, sizeof(buf) - 1);

    std::string		path(buf);
    return	path.substr(0, path.find_last_of("/\\"));
}

#else

#include	<unistd.h>
std::string	app_dir() {
    char buf[1024]	= {0};
    readlink("/proc/self/exe", buf, sizeof(buf) - 1);

    std::string		path(buf);
    return	path.substr(0, path.find_last_of("/\\"));
}

#endif
