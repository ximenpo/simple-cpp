#ifndef CSTR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CSTR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<string>

inline    const char* cstr(const char* pstr) {
    return  pstr;
}

inline    const char* cstr(const std::string& str) {
    return  str.c_str();
}

struct  cstr_converter {
    template<typename Type>
    const char* operator()(const Type str) {
        return  cstr(str);
    }
};

#endif
