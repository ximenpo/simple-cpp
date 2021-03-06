#ifndef PROPERTY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PROPERTY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <cstddef>


#define PROPERTY_SUPPORT(ClassName)       typedef  ClassName property_class__;


#define PROPERTY_SIMPLE(var, type)                                              \
type     var;

#if	defined(__GNUC__)
#define PROPERTY_OFFSET(STRUCT,MEMBER)	((size_t)((const char*)(&(((property_class__ *)1)->MEMBER))) - 1)
#else
#define PROPERTY_OFFSET(STRUCT,MEMBER)	offsetof(STRUCT,MEMBER)
#endif


#define PROPERTY(var, type, getter, setter)                                     \
struct property_##var                                                           \
{                                                                               \
    inline operator type()                                                      \
    {                                                                           \
        return (                                                                \
        (property_class__ *) ((char *)this - PROPERTY_OFFSET(property_class__,var))\
        )->getter();                                                            \
    }                                                                           \
    inline type operator=(const type & value)                                   \
    {                                                                           \
        (                                                                       \
        (property_class__ *) ((char *)this - PROPERTY_OFFSET(property_class__,var))\
        )->setter( value );                                                     \
        return value;                                                           \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


#define PROPERTY_READONLY(var, type, getter)                                    \
struct property_##var                                                           \
{                                                                               \
    inline operator type()                                                      \
    {                                                                           \
        return (                                                                \
        (property_class__ *) ((char *)this - PROPERTY_OFFSET(property_class__,var))\
        )->getter();                                                            \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


#define PROPERTY_WRITEONLY(var, type, setter)                                   \
struct property_##var                                                           \
{                                                                               \
    inline type operator=(const type & value)                                   \
    {                                                                           \
        (                                                                       \
        (property_class__ *) ((char *)this - PROPERTY_OFFSET(property_class__,var))\
        )->setter( value );                                                     \
        return value;                                                           \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


//
//	disable gcc compiler warnings.
//
#ifdef __GNUC__
#	pragma  GCC diagnostic ignored  "-Winvalid-offsetof"
#endif


#endif
