#include	<cassert>
#include	<cstdlib>

//
//	获取 nMin 到 nMax 之间 nPossible 可能性最大的 正态分布相似 随机数
//
//	nPlusTimes: 1 ~ 12，数字越大，随机数越集中于nPossible附近
//
int	rand_normal_distribution(int nMin, int nPossible, int nMax, int nPlusTimes = 3) {
    assert(nPlusTimes > 0);
    assert(nMin <= nPossible);
    assert(nMax >= nPossible);
    if(nMin == nMax) {
        return nMin;
    }

    double dValue = 0;
    for(int k = 0; k < nPlusTimes; ++k) {
        dValue += (double)rand() / RAND_MAX;// 0 ~ 1.0
    }
    dValue /= nPlusTimes;

    double	nLD	= nPossible - nMin;
    double	nRD	= nMax - nPossible;

    // 不在要求范围内，则按最可能的值进行处理
    int		nResult = nPossible;
    double	dRand	= 0;
    if(nRD < nLD) {
        if(dValue > 0.5 + nRD / (2 * nLD)) {
            dRand	= rand();
            dValue	= 0.5 + (dRand / RAND_MAX - 0.5) * (nRD / nLD);
        }
        nResult	= int(nMin + dValue * 2 * nLD + 0.5);
    } else if(nLD < nRD) {
        if(dValue < (nRD - nLD) / (2 * nRD)) {
            dRand	= rand();
            dValue = 0.5 - (dRand / RAND_MAX - 0.5) * (nLD / nRD);
        }
        nResult	= int(nMin + dValue * 2 * nRD + 0.5 - (nRD - nLD));
    } else {
        nResult	= int(nMin + dValue * 2 * nLD + 0.5);
    }

    assert(nResult >= nMin && nResult <= nMax);
    return nResult;
}
