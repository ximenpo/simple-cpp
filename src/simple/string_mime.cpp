
#include	<string>
#include	<memory>

#include	"simple/third/MimeCodes.cpp"

static inline bool is_base64_char(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

bool string_is_base64(const std::string& s) {
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

std::string		string_base64_encode(const unsigned char* str, unsigned long len) {
    std::string ret;
    ret.reserve(len * 2);
    base64_encode(str, str + len, std::back_inserter(ret));
    return  ret;
}

std::string		string_base64_decode(const unsigned char* str, unsigned long len) {
    std::string ret;
    ret.reserve(len);
    base64_decode(str, str + len, std::back_inserter(ret));
    return  ret;
}

std::string		string_qp_encode(const unsigned char* str, unsigned long len) {
    std::string ret;
    ret.reserve(len * 2);
    QpEncoder<const unsigned char*, std::back_insert_iterator<std::string> > e;
    e.Filter(std::back_inserter(ret), str, str + len);
    return  ret;
}

std::string		string_qp_decode(const unsigned char* str, unsigned long len) {
    std::string ret;
    ret.reserve(len);
    qp_decode(str, str + len, std::back_inserter(ret));
    return  ret;
}
