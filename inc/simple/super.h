#ifndef SUPER_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define SUPER_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

//
//	compatiable with msvc __super keyword.
//

#if	defined(_MSC_VER)
#	define		DECLARE_SUPER_CLASS(NAME)
#else
#	define		DECLARE_SUPER_CLASS(NAME)		typedef		NAME	__super;
#endif

#if	0	//	Usage:
class	SubClass : public SuperClass {
public:
    DECLARE_SUPER_CLASS(SuperClass);
public:
    void	Method() {
        __super::Method();
    }
};
#endif

#endif
