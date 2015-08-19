#ifndef CHECKSUM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define BYTE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include    "stdint.h"

inline  uint16_t    checksum (uint8_t *buf,int size) {
    uint16_t *buffer = (uint16_t*)buf;
    uint32_t cksum=0;
    while (size>1) {
        cksum +=*buffer++;
        size -=sizeof(uint16_t);
    }
    if (size) {
        cksum +=*(uint8_t *) buffer;
    }
    //将32位转换为16位
    while (cksum>>16) {
        cksum = (cksum>>16) + (cksum & 0xffff);
    }
    return uint16_t(~cksum);
}

#endif
