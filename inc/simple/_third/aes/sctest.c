/*
 *  String Crypt Test (Linux)
 *  Copyright (C) 2012
 *
 *  Contributors:
 *      Paul E. Jones <paulej@packetizer.com>
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

#include <stdio.h>
#include <string.h>
#include <unistd.h> // write

#include "AESStringCrypt.h"
#include "password.h"

/*
 * Main will just perform some simple test
 */
int main(int argc, char *argv[])
{
    char pass_input[MAX_PASSWD_LEN+1],
         pass[MAX_PASSWD_LEN+1],
         plaintext[1024],
         ciphertext[1024+83];
    int  passlen,
         plaintext_length,
         ciphertext_length;

    // Put in a bogus password
    strcpy(pass_input, "Hello");
    passlen = passwd_to_utf16(  pass_input,
                                strlen(pass_input),
                                MAX_PASSWD_LEN+1,
                                pass);

    // Fill in the plaintext 
    strcpy(plaintext,
           "This is a test to see if the encryption works right\n");

    // Encrypt the string
    ciphertext_length = AESStringCrypt(   pass,
                                            passlen,
                                            plaintext,
                                            strlen(plaintext),
                                            ciphertext);

    fprintf(stderr, "Ciphertext length was %i\n", ciphertext_length);

    // One could verify that the data encrypted properly
    //write(STDOUT_FILENO, ciphertext, ciphertext_length);

    // Decrypt the ciphertext
    strcpy(plaintext, "VOIDVOIDVOIDVOID"); // Just a dummy string
    plaintext_length = AESStringDecrypt(    pass,
                                           passlen,
                                           ciphertext,
                                           ciphertext_length,
                                           plaintext);

    fprintf(stderr, "Plaintext length was %i\n", plaintext_length);

    write(STDOUT_FILENO, plaintext, plaintext_length);

    return 0;
}
