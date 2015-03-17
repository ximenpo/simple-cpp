#ifndef RAND_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define RAND_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

//
//	获取 nMin 到 nMax 之间 nPossible 可能性最大的 正态分布相似 随机数
//
//	nPlusTimes: 1 ~ 12，数字越大，随机数越集中于nPossible附近
//
int	rand_normal_distribution(int nMin, int nPossible, int nMax, int nPlusTimes = 3);

#endif
