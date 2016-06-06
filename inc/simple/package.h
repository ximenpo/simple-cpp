#ifndef PACKAGE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PACKAGE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	"stdint.h"

//
//	package
//
struct	package {
    int32_t		cmd;
    uint32_t	flag;
    size_t		size;
    void*		data;
};

//
//	IO 工作接口
//
class	package_handler {
public:
    // 返回 -1：错误，返回0：成功发送，返回1：发送/接收过程中 
    virtual	int				read_package(package* pPkg)					= 0;
    virtual	int				write_package(package* pPkg)				= 0;

    virtual	package*		create_package(int32_t cmd, const void* data, size_t size, uint32_t flag = 0);
    virtual	void			destroy_package(package* pPkg);
};


#endif
