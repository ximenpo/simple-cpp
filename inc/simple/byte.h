#ifndef BYTE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define BYTE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include    "swap.h"

static  const int bsti = 1;     // Byte swap test integer

#define byte_big_endian()       ( (*(char*)&bsti) == 0 )
#define byte_little_endian()    ( (*(char*)&bsti) == 1 )

#define byte_hton_2(v)          ( byte_big_endian() ? v : swap_2_bytes(v) )
#define byte_hton_4(v)          ( byte_big_endian() ? v : swap_4_bytes(v) )
#define byte_hton_8(v)          ( byte_big_endian() ? v : swap_8_bytes(v) )

#define byte_ntoh_2(v)          ( byte_big_endian() ? v : swap_2_bytes(v) )
#define byte_ntoh_4(v)          ( byte_big_endian() ? v : swap_4_bytes(v) )
#define byte_ntoh_8(v)          ( byte_big_endian() ? v : swap_8_bytes(v) )

#endif
