#ifndef TYPE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TYPE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

template<typename InputType>
struct  type_inference {
    typedef InputType	original_type;
    typedef InputType	undecorated_type;
    enum	{
        is_ref		= false,
        is_const	= false,
    };
};

template<typename InputType>
struct  type_inference<InputType&> {
    typedef InputType&		original_type;
    typedef InputType		undecorated_type;
    enum	{
        is_ref		= true,
        is_const	= false,
    };
};

template<typename InputType>
struct  type_inference<const InputType> {
    typedef const InputType	original_type;
    typedef InputType		undecorated_type;
    enum	{
        is_ref		= false,
        is_const	= true,
    };
};

template<typename InputType>
struct  type_inference<const InputType&> {
    typedef const InputType&	original_type;
    typedef InputType			undecorated_type;
    enum	{
        is_ref		= true,
        is_const	= true,
    };
};

#endif
