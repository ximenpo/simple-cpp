#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include "des.h"

unsigned long ks[16][2];
unsigned char buf[8];

main() {
    unsigned long i;
    struct tms start,stop;
    double elapsed,s;

    deskey(ks,(unsigned char *)"12345678",0);
    printf("starting DES time trial, 1,000,000 encryptions\n");
    times(&start);

    for(i=0; i<1000000; i++)
        des(ks,buf);

    times(&stop);
    elapsed = (stop.tms_utime - start.tms_utime) / (double)CLK_TCK;
    printf("execution time = %.2f ms\n",elapsed);
    s = 1000000./elapsed;
    printf("%.1lf crypts/sec %.1lf bytes/sec %.1lf bits/sec\n",s,s*8,s*64);
    return 0;
}
