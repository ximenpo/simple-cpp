#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include "des.h"

DES3_KS ks;
unsigned char buf[8];

main()
{
	unsigned long i;
	struct tms start,stop;
	double elapsed,s;

	des3key(ks,(unsigned char *)"123456789abcdef012345678",0);
	printf("starting triple-DES time trial, 1,000,000 encryptions\n");
	times(&start);

	for(i=0;i<1000000;i++)
		des3(ks,buf);

	times(&stop);
	elapsed = (stop.tms_utime - start.tms_utime) / (double)CLK_TCK;
	printf("execution time = %.2f s\n",elapsed);
	s = 1000000 / elapsed;
	printf("%.1lf crypts/sec %.1lf bytes/sec %.1lf bits/sec\n",
	       s,s*8,s*64);
	return 0;
}
