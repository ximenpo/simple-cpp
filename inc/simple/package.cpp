
#include	<string.h>
#include	"package.h"


package* package_handler::create_package(int32_t cmd, const void* data, size_t size, uint32_t flag) {
    package*	pkg	= new package;

    pkg->cmd	= cmd;
    pkg->flag	= flag;

    if(0 != data && size > 0) {
        pkg->data	= new char[(size/16 + 1) * 16];
        pkg->size	= size;

        memcpy(pkg->data, data, size);
    } else {
        pkg->data	= 0;
        pkg->size	= 0;
    }

    return	pkg;
}

void package_handler::destroy_package(package* pkg) {
    if(0 != pkg) {
        delete	pkg->data;
        delete	pkg;
    }
}

