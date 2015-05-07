
#include	<string>
#include	<memory>
#include	<iterator>

#include	"_third/MimeCodes.cpp"
#include	"_third/url_parser.h"

bool  string_parse_url(const char* url,
                       std::string *scheme,
                       std::string *host,
                       std::string *port,
                       std::string *path,
                       std::string *query,
                       std::string *fragment,
                       std::string *username,
                       std::string *password) {
    parsed_url* items = parse_url(url);
    if(0 == items) {
        return false;
    }

    if(0 != scheme) {
        scheme->clear();
        if(0 != items->scheme)
            *scheme = items->scheme;
    }
    if(0 != host) {
        host->clear();
        if(0 != items->host)
            *host = items->host;
    }
    if(0 != port) {
        port->clear();
        if(0 != items->port)
            *port = items->port;
    }
    if(0 != path) {
        path->clear();
        if(0 != items->path)
            *path = items->path;
    }
    if(0 != query) {
        query->clear();
        if(0 != items->query)
            *query = items->query;
    }
    if(0 != fragment) {
        fragment->clear();
        if(0 != items->fragment)
            *fragment = items->fragment;
    }
    if(0 != username) {
        username->clear();
        if(0 != items->username)
            *username = items->username;
    }
    if(0 != password) {
        password->clear();
        if(0 != items->password)
            *password = items->password;
    }

    parsed_url_free(items);
    return true;
}

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
