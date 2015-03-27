/*
 * pwgen.c 1.4
 *
 *   Usage: pwgen [length] [specials]
 *
 *          length - an optional argument indicating the length of the password
 *          specials - use '1' to force use of special characters
 *
 * This will generate random passwords of the specified or default length.
 * If the second argument is a 1 then special characters will also be used.
 *
 * Copyright (C) 2013 - Paul E. Jones <paulej@packetizer.com>
 * Permission to use, copy, modify and distribute this software is granted.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#include <Wincrypt.h>
#else
#include <malloc.h>
#include <time.h>
#endif

/* Define the default password length */
#define DEFAULT_PASSWORD_LENGTH 16

/*
 * GeneratePassword
 *
 *     Description
 *         This routine will generate a password and return it as a string.
 *         By default, it will not utilize special characters like "~" in
 *         passwords, but if the second argument is a 1, it will.  Note that
 *         use of special characters provides only minimum additional strenth,
 *         yet they are not very friendly for humans. For details, visit
 *         https://secure.packetizer.com/pwgen/.
 *
 *     Parameters
 *         length [in]
 *             The length of the password
 *         special [in]
 *             Indicates whether to use special characters other than
 *             the letters A-Z, a-z, and digits 0-9.
 *         password [out]
 *             Character array containing the produced password
 *
 *     Returns
 *         Return 0 if successful or non-zero if there was a failure.
 */
int GeneratePassword(   int length,
                        int special,
                        char *password)
{
    char pwchars[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
        'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z', '~', '`', '!', '@', '#', '$', '%', '^',
        '&', '*', '(', ')', '_', '+', '=', '-', '{', '}', '|', '\\', ']', '[',
        ':', '"', '\'', ';', '<', '>', '?', '/', '.'
    };
    int max_random;
    int random_value = 0;
#ifdef WIN32
    HCRYPTPROV hProv;
    DWORD result_code;
#else
    FILE *fp = NULL;
#endif

    /*
     * To avoid modulo bias in selecting random numbers, we will discard
     * any random number greater than max_random.  This value of max_random
     * is determined as per the equations shown below.
     */
    if (special == 1)
    {
        max_random = 255 - (255 % 93) - 1;
    }
    else
    {
        max_random = 255 - (255 % 62) - 1;
    }

    /*
     * Prepare to generate random numbers
     */
#ifdef WIN32
    if (!CryptAcquireContext(   &hProv,
                                NULL,
                                NULL,
                                PROV_RSA_FULL,
                                CRYPT_VERIFYCONTEXT))
    {
        result_code = GetLastError();
        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContext(   &hProv,
                                        NULL,
                                        NULL,
                                        PROV_RSA_FULL,
                                        CRYPT_NEWKEYSET|CRYPT_VERIFYCONTEXT))
            {
                result_code = GetLastError();
            }
            else
            {
                result_code = ERROR_SUCCESS;
            }
        }

        if (result_code != ERROR_SUCCESS)
        {
            fprintf(stderr, "Could not acquire handle to crypto context");
            return -1;
        }
    }
#else
    /*
     * We'll use /dev/urandom. It is less preferred over /dev/random,
     * but /dev/random blocks and can take a long time to return
     * a random number.  If /dev/urandom cannot be accessed, we'll use
     * the rand() function in the C libraries.
     */
    if (!(fp = fopen("/dev/urandom", "r")))
    {
        /* We must use the pseudo random number generator */
        srand(time(NULL));
    }
#endif

    while(length > 0)
    {
#ifdef WIN32
        if (!CryptGenRandom(hProv,
                            1,
                            (BYTE *) &random_value))
        {
            fprintf(stderr, "Windows is unable to generate random digits");
            CryptReleaseContext(hProv, 0);
            return -1;
        }
#else
        random_value = (fp) ? fgetc(fp) : rand();
#endif

        /*
         * Reject certain random numbers to avoid modulo bias
         */
        if (random_value > max_random)
        {
            continue;
        }

        if (special == 1)
        {
            *password = pwchars[random_value % 93];
        }
        else
        {
            *password = pwchars[random_value % 62];
        }
        password++;
        length--;
    }

    /* Insert an end-of-line character */
    *password = '\0';

#ifdef WIN32
    CryptReleaseContext(hProv, 0);
#else
    if (fp)
    {
        fclose(fp);
    }
#endif

    return 0;
}

/*
 * main
 *
 *     Description
 *         This is the main routine.
 *
 *     Parameters
 *         argc [in]
 *             The number of command-line arguments
 *         argv [in]
 *             The command-line argument array
 *
 *     Returns
 *         Return 0 if successful or non-zero if there was a failure.
 *
 */
int main(int argc, char **argv)
{
    int password_length = 0;
    char *password;
    int result_code = 0;
    int use_special = 0;

    /*
     * Grab the requested password length from the command-line
     */ 
    if (argc > 1)
    {
        password_length = atoi(argv[1]);
    }
    if (argc > 2)
    {
        if (atoi(argv[2]) == 1)
        {
            use_special = 1;
        }
    }

    /*
     * If we are given no specified length or something silly, use
     * the default password length to produce the password
     */
    if (!(password_length > 0))
    {
        password_length = DEFAULT_PASSWORD_LENGTH;
    }

    /* Allocate space for the password */
    password = malloc(password_length + 1);
    if (password != NULL)
    {
        if (!GeneratePassword(password_length,use_special,password))
        {
            printf("%s\n", password);
        }
        else
        {
            result_code = 2;
        }
        free(password);
    }
    else
    {
        result_code = 1;
    }

    return result_code;
}
