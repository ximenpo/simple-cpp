#ifndef PROPERTY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PROPERTY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <cstddef>


#define PROPERTY_SUPPORT(ClassName)       typedef  ClassName property_class__;


#define PROPERTY_SIMPLE(var, type)                                              \
type     var;


#define PROPERTY(var, type, get_func, put_func)                                 \
struct property_##var                                                           \
{                                                                               \
    inline operator type()                                                      \
    {                                                                           \
        return (                                                                \
        (property_class__ *) ((char *)this - offsetof(property_class__,var))    \
        )->get_func();                                                          \
    }                                                                           \
    inline type operator=(const type & value)                                   \
    {                                                                           \
        (                                                                       \
        (property_class__ *) ((char *)this - offsetof(property_class__,var))    \
        )->put_func( value );                                                   \
        return value;                                                           \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


#define PROPERTY_READONLY(var, type, get_func)                                  \
struct property_##var                                                           \
{                                                                               \
    inline operator type()                                                      \
    {                                                                           \
        return (                                                                \
        (property_class__ *) ((char *)this - offsetof(property_class__,var))    \
        )->get_func();                                                          \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


#define PROPERTY_WRITEONLY(var, type, put_func)                                 \
struct property_##var                                                           \
{                                                                               \
    inline type operator=(const type & value)                                   \
    {                                                                           \
        (                                                                       \
        (property_class__ *) ((char *)this - offsetof(property_class__,var))    \
        )->put_func( value );                                                   \
        return value;                                                           \
    }                                                                           \
};                                                                              \
friend struct property_##var;                                                   \
property_##var var;


//
//	disable compiler warnings.
//
#ifdef __GNUC__
#	pragma  GCC diagnostic ignored  "-Winvalid-offsetof"
#endif


#endif
