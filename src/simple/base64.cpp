
#include	"simple/third/base64.cpp"

bool base64_check(const std::string& s) {
    size_t nLen	= s.size();

    if(nLen < 3) {
        return false;
    }

    if('=' == s[nLen - 1])nLen--;
    if('=' == s[nLen - 1])nLen--;

    size_t i = 0;
    for(; i < nLen; ++i) {
        if(!is_base64(s[i])) {
            return false;
        }
    }
    return true;
}
