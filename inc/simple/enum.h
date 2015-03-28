#ifndef ENUM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define ENUM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

//
// 枚举集合，类似特定版本的 std::bitset
//
template<typename EnumType>
class enum_set {
public:
    enum_set(unsigned int data = 0):data_(data) {}
    operator	unsigned long() {
        return	data_;
    }

    //	set/unset/is_set
    void	set(EnumType item) {
        data_|=(1<<item);
    }
    void	unset(EnumType item) {
        data_&=~(1<<item);
    }
    bool	is_set(EnumType item)const {
        return (0!=(data_&(1<<item)));
    }

    //	template optimize
    template<EnumType item>void	set() {
        data_|=(1<<item);
    }
    template<EnumType item>void	unset() {
        data_&=~(1<<item);
    }
    template<EnumType item>void	set(bool bset) {
        bset?set<item>():unset<item>();
    }
    template<EnumType item>bool	is_set()const {
        return (0!=(data_&(1<<item)));
    }

    //	internal data operations.
    unsigned long	data()const {
        return data_;
    }
    void	assign(unsigned long data) {
        data_ = data;
    }
    void	clear() {
        data_ = 0;
    }

private:
    unsigned long	data_;
};

//
// 枚举转换为字符串处理
//
#define SET_ENUM_STRINGS(TYPE, VALUES)	\
	inline const char* const enum_to_string(const TYPE index)\
{\
	const static char* TYPE##EnumStrings[] = {VALUES ""};\
	return (index >= 0 && (unsigned int)(index) < sizeof(TYPE##EnumStrings)/sizeof(TYPE##EnumStrings[0]))?TYPE##EnumStrings[index]:"";\
}
#define	ENUM_NAME(Index)	(#Index),
#define	ENUM_ITEM(VALUE)	(VALUE),

#define SET_ENUM_STRINGS_EX(TYPE, VALUES)	\
	inline const char* const enum_to_string(const TYPE index)\
{\
	switch(index)\
{\
	VALUES \
	default:return "";\
}\
}

#define	ENUM_NAME_EX(Index)	        case (Index):{return #Index;}
#define	ENUM_ITEM_EX(Index,VALUE)	case (Index):{return VALUE;}

#endif
