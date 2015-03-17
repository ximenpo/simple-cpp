#ifndef ARRAY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define ARRAY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	<stdexcept>

//
//	安全的一维数组
//
template <class	ArrayType,	int	ArraySize>
struct safe_array {
public:
    class ptr {
    public:
        ptr(safe_array&	array, int pos)
            :	pos_(pos), array_(array) {
        }

        ArrayType&	operator*() {
            return array_[pos_];
        }

        const ArrayType& operator*()const {
            return array_[pos_];
        }

        bool operator==(const ptr& p) const {
            return &this->array_ ==	&p.array_ && this->pos_	== p.pos_;
        }

        bool operator!=(const ptr& p) const {
            return &this->array_ !=	&p.array_ || this->pos_	!= p.pos_;
        }

        const ptr operator + (int offset)const {
            return ptr(array_, pos_	+ offset);
        }

        const ptr operator - (int offset)const {
            return ptr(array_, pos_	- offset);
        }

        const ptr operator += (int offset) {
            pos_ +=	offset;
            return *this;
        }

        const ptr& operator	-= (int	offset) {
            pos_ -=	offset;
            return *this;
        }

        ptr	operator++(int) {
            ptr	pTemp =	*this;
            ++pos_;
            return pTemp;
        }

        ptr& operator++() {
            ++pos_;
            return *this;
        }
    private:
        int	pos_;
        safe_array&	array_;
    };

public:
    safe_array() {
        for(int i = 0; i < ArraySize; ++i) {
            array_data_[i]	= ArrayType();
        }
    }

    ArrayType&	operator []	(int index) {
        assert(is_valid_array_index(index));

        if(!is_valid_array_index(index)) {
            throw	std::out_of_range("invalid array index");
        }

        return	array_data_[index];
    }

    const ArrayType& operator [] (int index)const {
        assert(is_valid_array_index(index));

        if(!is_valid_array_index(index)) {
            throw	std::out_of_range("invalid array index");
        }

        return	array_data_[index];
    }

    operator	void*() {
        return array_data_;
    }

    ArrayType&	operator*() {
        return array_data_[0];
    }

    const ArrayType& operator*()const {
        return array_data_[0];
    }

    operator  ptr() {
        return ptr(*this, 0);
    }

    operator  const	ptr()const {
        return ptr(*this, 0);
    }

    ptr	operator + (int	offset) {
        return ptr(*this, offset);
    }

    const ptr operator + (int offset)const {
        return ptr(*this, offset);
    }

    ptr	operator - (int	offset) {
        return ptr(*this, -offset);
    }

    const ptr operator - (int offset)const {
        return ptr(*this, -offset);
    }
private:
    //Note:	array format: TYPE Name[element]
    ArrayType	array_data_[ArraySize];
    bool		is_valid_array_index(int index)const {
        return	index >= 0 && index	< ArraySize;
    }
};

#if	!defined(ARRAY)
#	if	defined(NDEBUG)
#		define	ARRAY(TYPE, NAME, SIZE)		TYPE NAME[SIZE]
#	else
#		define	ARRAY(TYPE, NAME, SIZE)		safe_array<TYPE,SIZE>	NAME
#	endif
#endif

#define	ARRAY_LENGTH(a)						(sizeof(a) / sizeof(a[0]))

#endif
