#ifndef BIT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define BIT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <cstring>

#if	defined(_MSC_VER) && _MSC_VER >= 1400	// VS2005
#	include	<intrin.h>
#endif

//	run for 32bit system.
enum {
    BITS_COUNT	= 32,
};

inline	int bit_count(unsigned int value) {
//#define bit_count_POW__(c) (1<<(c))
//#define bit_count_MASK__(c) (((unsigned long)-1) / (bit_count_POW__(bit_count_POW__(c)) + 1))
//#define bit_count_ROUND__(n, c) (((n) & bit_count_MASK__(c)) + ((n) >> bit_count_POW__(c) & bit_count_MASK__(c)))
//
//    value = bit_count_ROUND__(value, 0);
//    value = bit_count_ROUND__(value, 1);
//    value = bit_count_ROUND__(value, 2);
//    value = bit_count_ROUND__(value, 3);
//    value = bit_count_ROUND__(value, 4);
//    return n;
//
//#undef bit_count_POW__
//#undef bit_count_MASK__
//#undef bit_count_ROUND__
    value = (value & 0x55555555U) + ((value >> 1 ) & 0x55555555U);
    value = (value & 0x33333333U) + ((value >> 2 ) & 0x33333333U);
    value = (value & 0x0F0F0F0FU) + ((value >> 4 ) & 0x0F0F0F0FU);
    value = (value & 0x00FF00FFU) + ((value >> 8 ) & 0x00FF00FFU);
    value = (value & 0x0000FFFFU) + ((value >> 16) & 0x0000FFFFU);
    return value;
}

inline	size_t bit_lowest_one_index(register unsigned long bits) {
#ifdef __GNUC__
    if(bits) {
        return __builtin_ctz(bits);
    }
#elif defined(_MSC_VER) && _MSC_VER >= 1400	// VS2005
    unsigned long idx;
    if(_BitScanForward(&idx, bits)) {
        return idx;
    }
#endif
    // SWAR (SIMD Within A Register) algorithms for manipulating bits in an integer
    // default implementation works best for 32-bit unsigned integer (unsigned long).
    //  Specializations should be created to optimize other types
    register unsigned long x	= bits;  // Replicate the lowest 'one' bit into higher positions
    {
        x |= (x << 1);
        x |= (x << 2);
        x |= (x << 4);
        x |= (x << 8);
        x |= (x << 16);
    }
    register unsigned long y	= ~x; // 32-bit recursive reduction using SWAR...
    {
        y -= ((y >> 1) & 0x55555555);                   // first step is mapping 2-bit values into sum of 2 1-bit values
        y = (((y >> 2) & 0x33333333) + (y & 0x33333333));
        y = (((y >> 4) + y) & 0x0f0f0f0f);
        y += (y >> 8);
        y += (y >> 16);
        y &= 0x0000003f;
    }
    return	y;
}


/*!
    \class    bit_token
    \brief    An array of bits with fast search for the first zero bit
    \author   Ted Nguyen

  This class manages an array of bits in a hierarchy of masks that optimize
  the search for the first zero bit. This class uses template recursion to
  implement each level of the hierarchy. Each level holds an array of bits
  where a zero indicates that a word in the derived level contains all zeros.

  template parameters:
    max_bits : maximum number of bits managed by this array.
*/
#ifdef __GNUC__
#	include	"_impls/bit_token_data.inc"
#endif

template <size_t max_bits>
class bit_token {
#ifndef __GNUC__
#	include	"_impls/bit_token_data.inc"
#endif
public:
    bit_token() : array_() {
    }
    explicit bit_token(bool x) {
        array_.assign(x);
    }
    void assign(bool x) {
        array_.assign(x);
    }
    static size_t size() {
        return max_bits;
    }
    bool operator[](size_t n) const {
        return get(n);
    }
    bool get(size_t n)  const {
        return array_.get(n);
    }
    size_t get_token() const {
        return array_.find_zero();	// find first zero bit
    }
    void set(size_t n, bool x) {
        array_.set(n, x);			// set one bit at index n to x
    }

protected:
    bit_token_data<((max_bits+BIT_ARRAY_BITS_MASK) >> BIT_ARRAY_BITS_SHIFT)>   array_;      // round-up then convert to words
};

#endif
