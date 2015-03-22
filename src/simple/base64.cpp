
#include	<string>
#include	<memory>

#include	"simple/third/mime.h"

static inline bool is_base64_char(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

bool base64_check(const std::string& s) {
    size_t nLen	= s.size();

    if(nLen < 3) {
        return false;
    }

    if('=' == s[nLen - 1])nLen--;
    if('=' == s[nLen - 1])nLen--;

    size_t i = 0;
    for(; i < nLen; ++i) {
        if(!is_base64_char(s[i])) {
            return false;
        }
    }
    return true;
}


std::string		base64_encode(const unsigned char* str, unsigned int len) {
    std::auto_ptr<char>	buf(new char[len * 2 + 1]);
    mime::Base64Encoder<const unsigned char*, char*>	encoder;
    char*	end	= encoder.filter(str, str + len, buf.get(), true);
    *end	= 0;
    return	std::string(buf.get(), end - buf.get());
}

std::string		base64_decode(const std::string& str) {
    std::auto_ptr<char>	buf(new char[str.size() + 1]);
    mime::Base64Decoder<const char*, char*>	encoder;
    char*	end	= encoder.filter(str.c_str(), str.c_str() + str.size(), buf.get(), true);
    *end	= 0;
    return	std::string(buf.get(), end - buf.get());
}
