#define DEBUG 1
/* Encrypt/decrypt with triple DES */
#include <stdio.h>
#include "des.h"
#include "md5.h"

void gethex(char *result,char *cp,int cnt);
void memxor(char *,char *,int);
int doencrypt(DES3_KS,int cbc,char iv[8],FILE *in,FILE *out);
int dodecrypt(DES3_KS,int cbc,char iv[8],FILE *in,FILE *out);

main(int argc,char *argv[]) {
    int c,cnt,encrypt,decrypt,hexflag,tripflag,md5flag,keylen,cbc;
    register int i;
    char key[24],tkey1[256],tkey2[256],iv[8],*akey,*getpass();
    MD5_CTX mdcontext;
    DES3_KS ks;
    extern char *optarg;

    keylen = 8;
    tripflag = hexflag = encrypt = decrypt = md5flag = 0;
    cbc = 1;
    akey = NULL;
    while((c = getopt(argc,argv,"53hedk:b")) != EOF) {
        switch(c) {
        case 'e':
            encrypt++;
            break;
        case 'd':
            decrypt++;
            break;
        case 'k':
            akey = optarg;
            break;
        case 'b':
            cbc = 0;
            break;
        }
    }
    if(encrypt == 0 && decrypt == 0) {
        fprintf(stderr,"Usage: des -e|-d [-b] [-k key]\n");
        exit(2);
    }
    if(akey == NULL) {
        /* No key on command line, prompt for it */
        memset(tkey1,0,sizeof(tkey1));
        memset(tkey2,0,sizeof(tkey2));
        for(;;) {
            akey = getpass("Enter key: ");
            strncpy(tkey1,akey,sizeof(tkey1));
            akey = getpass("Enter key again: ");
            strncpy(tkey2,akey,sizeof(tkey2));
            if(strncmp(tkey1,tkey2,sizeof(tkey1)) != 0) {
                fprintf(stderr,"Key mistyped, try again\n");
            } else
                break;
        }
        akey = tkey1;
    }
    MD5Init(&mdcontext);
    MD5Update(&mdcontext,akey,strlen(akey));
    MD5Final(&mdcontext);
    memcpy(key,mdcontext.digest,16);
    memcpy(key+16,mdcontext.digest,8);

    /* Blot out original key */
    memset(akey,0,strlen(akey));

    des3key(ks,key,!encrypt);

    /* Initialize IV to all zeros */
    memset(iv,0,8);

    return ( encrypt ? doencrypt(ks,cbc,iv,stdin,stdout)
             : dodecrypt(ks,cbc,iv,stdin,stdout) );
}
/* Encrypt standard input to standard output */
int
doencrypt(DES3_KS ks,int cbc,char iv[8],FILE *in,FILE *out) {
    char work[8];
    int cnt,i;

    for(;;) {
        if((cnt = fread(work,1,8,in)) != 8) {
            /* Put residual byte count in the last block.
             * Note that garbage is left in the other bytes,
             * if any; this is a feature, not a bug, since it'll
             * be stripped out at decrypt time.
             */
            work[7] = cnt;
        }
        if(cbc) {
            /* CBC mode; chain in last cipher word */
            memxor(work,iv,8);
        }
        des3(ks,work);	/* Encrypt block */
        if(cbc) {	/* Save outgoing ciphertext for chain */
            memcpy(iv,work,8);
        }
        fwrite(work,1,8,out);
        if(cnt != 8)
            break;
    }
    return 0;
}
int
dodecrypt(DES3_KS ks,int cbc,char iv[8],FILE *in,FILE *out) {
    char work[8],nwork[8],ivtmp[8];
    int cnt,i;

    cnt = fread(work,1,8,in);	/* Prime the pump */
    for(;;) {
        if(cbc) {	/* Save incoming ciphertext for chain */
            memcpy(ivtmp,work,8);
        }
        des3(ks,work);
        if(cbc) {	/* Unchain block, save ciphertext for next */
            memxor(work,iv,8);
            memcpy(iv,ivtmp,8);
        }
        /* Save buffer pending next read */
        memcpy(nwork,work,8);
        /* Try to read next block */
        cnt = fread(work,1,8,stdin);
        if(cnt != 8) {	/* Can "only" be 0 if not 8 */
            /* Prev block was last one, write appropriate number
             * of bytes
             */
            cnt = nwork[7];
            if(cnt < 0 || cnt > 7) {
                fprintf(stderr,"Corrupted file or wrong key\n");
            } else if(cnt != 0)
                fwrite(nwork,1,cnt,stdout);
            exit(0);
        } else {
            /* Now okay to write previous buffer */
            fwrite(nwork,1,8,out);
        }
    }
    return 0;
}
/* Convert hex/ascii nybble to binary */
int
htoa(char c) {
    if(c >= '0' && c <= '9')
        return c - '0';
    if(c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if(c >= 'A' && c <= 'F')
        return 10 + c - 'A';
    return -1;
}
/* Convert bytes from hex/ascii to binary */
void
gethex(char *result,char *cp,int cnt) {
    while(cnt-- != 0) {
        *result = htoa(*cp++) << 4;
        *result++ |= htoa(*cp++);
    }
}
void
memxor(char *dest,char *src,int cnt) {
    int i;

    for(i=0; i<cnt; i++)
        dest[i] ^= src[i];
}

#ifdef	DEBUG
put24(cp)
register char *cp;
{
    int i;

    for(i=0; i<24; i++) {
        fprintf(stderr,"%02x ",*cp++ & 0xff);
    }
}
#endif

