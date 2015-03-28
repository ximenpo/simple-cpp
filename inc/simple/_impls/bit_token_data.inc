
enum {
#if defined(_WIN64) ||  defined(__x86_64__)
    BIT_ARRAY_BITS_SHIFT	= 6,				// used in rounding shifts
#else
    BIT_ARRAY_BITS_SHIFT	= 5,				// used in rounding shifts
#endif
    BIT_ARRAY_BITS_MASK		= BITS_COUNT - 1,	// used in rounding masks
};

template <size_t Words>
class bit_token_data : protected bit_token_data< ((Words+BIT_ARRAY_BITS_MASK)>>BIT_ARRAY_BITS_SHIFT) > {
#if defined(_WIN64) ||  defined(__x86_64__)
    typedef unsigned long long  word_type;
#else
    typedef unsigned long word_type;
#endif
    typedef bit_token_data<((Words+BIT_ARRAY_BITS_MASK) >> BIT_ARRAY_BITS_SHIFT)>  base_type;
    word_type words_[Words];      // storage of bits in this stage
public:
    bit_token_data() {
        assign(false);
    }
    void	assign(bool x) {
        memset(this, x ? 0xff : 0x00, sizeof(bit_token_data));
    }
    bool	get(size_t n)  const {
        return 0 != (words_[n>>BIT_ARRAY_BITS_SHIFT] & ((word_type)1 << (n&BIT_ARRAY_BITS_MASK)));
    }
    size_t	find_zero() const {	// find first zero bit (template recursive )
        size_t i = base_type::find_zero();
        return (i << BIT_ARRAY_BITS_SHIFT) + (i < Words ? bit_lowest_one_index(~words_[i]) : 0);    // one branch here
    }
    void	set(size_t n, bool x = true) {			// set one bit at index n to x  (template recursive)
        size_t pos = n & BIT_ARRAY_BITS_MASK;   // bit position
        n >>= BIT_ARRAY_BITS_SHIFT;             // convert to index of word containing bit
        word_type& word = *(words_ + n);
        word &= ~((word_type)1 << pos);			// clear bit
        word |=   (word_type)x << pos;			// set bit if it is non-zero
        base_type::set(n, ~word == 0);			// update n bit in base to one if word contains all ones
    }
};

template <> class bit_token_data<1u> { // full specialization of the final stage
#if defined(_WIN64) ||  defined(__x86_64__)
    typedef unsigned long long  word_type;
#else
    typedef unsigned long word_type;
#endif
    word_type word_;
public:
    bit_token_data() : word_(0) {
    }
    void	assign(bool x) {
        word_ = x ? ~(word_type)0 : 0;
    }
    bool	get(size_t n)  const {
        return 0 != (word_ & ((word_type)1 << (n&BIT_ARRAY_BITS_MASK)));
    }
    size_t	find_zero() const {
        return bit_lowest_one_index(~word_);
    }
    void	set(size_t n, bool x) { // set one bit at index n to x
        n &= BIT_ARRAY_BITS_MASK;                     // bit position
        word_ &= ~((word_type)1 << n); // clear bit
        word_ |=   (word_type)x << n;  // set bit if it is non-zero
    }
};
