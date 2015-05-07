/*
 * AESStringCrypt.h 1.0
 *
 * Copyright (C) 2007, 2008, 2009, 2012
 *
 * This software is licensed as "freeware."  Permission to distribute
 * this software in source and binary forms is hereby granted without a
 * fee.  THIS SOFTWARE IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * THE AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DAMAGES RESULTING FROM
 * THE USE OF THIS SOFTWARE, EITHER DIRECTLY OR INDIRECTLY, INCLUDING,
 * BUT NOT LIMITED TO, LOSS OF DATA OR DATA BEING RENDERED INACCURATE.
 *
 */

#ifndef __STRINGCRYPT_H
#define __STRINGCRYPT_H

#include "aes.h"
#include "sha256.h"

typedef unsigned char sha256_t[32];


unsigned long long AESStringCrypt(  char *password,
                                    unsigned long password_length,
                                    char *plaintext,
                                    unsigned long long plaintext_length,
                                    char *ciphertext);

unsigned long long AESStringDecrypt(char *password,
                                    unsigned long password_length,
                                    char *ciphertext,
                                    unsigned long long ciphertext_length,
                                    char *plaintext);

#endif // __STRINGCRYPT_H
