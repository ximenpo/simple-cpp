#ifndef BASE64_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define BASE64_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <string>

bool			base64_check(const std::string& s);
std::string		base64_encode(const unsigned char* s, unsigned int len);
std::string		base64_decode(const std::string& s);

inline	std::string	base64_encode(const char* s, unsigned int len) {
    return	base64_encode((const unsigned char*)s, len);
}

inline	std::string	base64_encode(const std::string& s) {
    return	base64_encode(s.c_str(), s.size());
}

#endif
