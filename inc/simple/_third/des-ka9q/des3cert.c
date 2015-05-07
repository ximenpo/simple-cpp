#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "des.h"

static void get8 (unsigned char *);
static void put8 (unsigned char *);

main()
{
	unsigned char key[24],plain[8],cipher[8],work[8];
	int test,fail;
	static DES3_KS kne,knd;

	for(test=0;!feof(stdin);test++){
		fail = 0;

		get8(key);
		memcpy(&key[8],key,8); /* K1=K2=K3 for test */
		memcpy(&key[16],key,8);
		printf(" K: "); put8(key);
		des3key(kne,key,0);
		des3key(knd,key,1);

		get8(plain);
		printf(" P: "); put8(plain);

		get8(cipher);
		printf(" C: "); put8(cipher);

		memcpy(work,plain,8);
		des3(kne,work);

		if(memcmp(work,cipher,8) !=0){
			printf(" Encrypt FAIL");
			printf(" c: "); put8(work);
			fail++;
		}
		des3(knd,work);
		if(memcmp(work,plain,8) != 0){
			printf(" Decrypt FAIL");
			printf(" p: "); put8(work);
			fail++;
		}
		if(fail == 0)
			printf(" OK");
		printf("\n");
	}
	return 0;
}
static void
get8(cp)
unsigned char *cp;
{
	int i,t;

	for(i=0;i<8;i++){
		scanf("%2x",&t);
		if(feof(stdin))
			exit(0);
		*cp++ = t;
	}
}
static void
put8(cp)
unsigned char *cp;
{
	int i;

	for(i=0;i<8;i++){
		printf("%02x",*cp++ & 0xff);
	}
}
