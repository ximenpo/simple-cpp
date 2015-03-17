#ifndef POW2_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define POW2_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

// Is a power of 2
inline bool is_pow2(int n) {
    if (n<=0) {
        return false;
    }
    return (n & (n-1))==0;
}

// Next power of 2
inline int pow2_next(int n) {
    if (n<=0) {
        return 1;
    }
    //n--;
    n |= n>>1;
    n |= n>>2;
    n |= n>>4;
    n |= n>>8;
    n |= n>>16;
    n++;
    return n;
}

// Previous power of 2, not include itself
inline int pow2_prev(int n) {
    if ( n <= 2 ) {
        return 1;
    }

    if ( n <= 4 ) {
        return 2;
    }

    if( is_pow2(n) ) {
        return n>>1;
    }

    int n2 = n>>1;  // divide by 2
    return pow2_next(n2);
}

// Nearest power of 2
inline int pow2_near(int n) {
    if (is_pow2(n)) {
        return n;
    }

    int nNext = pow2_next(n);
    int nPrev = pow2_prev(n);
    if (nNext-n>abs(n-nPrev)) {
        return nPrev;
    } else {
        return nNext;
    }
}

#endif
