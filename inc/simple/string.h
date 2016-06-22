#ifndef STRING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cstdarg>
#include	<map>
#include	<deque>
#include	<iterator>
#include	<string>
#include	<sstream>
#include	<algorithm>

//
//	trim & ltrim & rtrim
//
inline	void	string_ltrim(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t\r\n"));
}
inline	void	string_rtrim(std::string& str) {
    str.erase(str.find_last_not_of(" \t\r\n")	+ 1);
}
inline	void	string_trim(std::string& str) {
    string_ltrim(str);
    string_rtrim(str);
}
inline	std::string		string_ltrim(const char* str) {
    std::string	s(str);
    string_ltrim(s);
    return	s;
}
inline	std::string		string_rtrim(const char* str) {
    std::string	s(str);
    string_rtrim(s);
    return	s;
}
inline	std::string		string_trim(const char* str) {
    std::string	s(str);
    string_trim(s);
    return	s;
}

//
//	upper & lower
//
inline	void	string_toupper(std::string&	str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}
inline	void	string_tolower(std::string&	str) {
    std::transform(str.begin(), str.end(), str.begin(), ::towlower);
}
inline	std::string		string_toupper(const char* str) {
    std::string	s(str);
    string_toupper(s);
    return	s;
}
inline	std::string		string_tolower(const char* str) {
    std::string	s(str);
    string_tolower(s);
    return	s;
}

//
//	hex
//
inline	void			string_hex(unsigned char c, char* buf_hex, bool lower_case = false) {
    const char* HEX_TOKENS	= lower_case ? "0123456789abcdef\0": "0123456789ABCDEF\0";
    buf_hex[0]	= HEX_TOKENS[c >> 4 & 0x0F];
    buf_hex[1]	= HEX_TOKENS[c >> 0 & 0x0F];
    buf_hex[2]	= '\0';
}
inline	std::string		string_hex(unsigned char c, bool lower_case = false) {
    char	buf[4]	= {0};
    string_hex(c, buf, lower_case);
    return	std::string(buf);
}

//
//	format
//
void			string_format(std::string& str, const char* fmt, ...);
std::string		string_format(const char* fmt, ...);

//
//	join
//
template<typename InputIterator>
std::ostream&		string_join(std::ostream& os, InputIterator begin, InputIterator end, const std::string& delimeter, const std::string& prefix, const std::string& postfix) {
    os	<<	prefix;
    for(; begin != end; ) {
        os	<<	*begin;
        if(++begin != end) {
            os	<<	delimeter;
        }
    }
    return	os	<<	postfix;
}

template<typename InputIterator>
std::string		string_join(InputIterator begin, InputIterator end, const std::string& delimeter, const std::string& prefix = "", const std::string& postfix = "") {
    std::ostringstream	os;
    string_join(os, begin, end, delimeter, prefix, postfix);
    return	os.str();
}

//
//	split
//
template<typename OutputIterator>
void	string_split(const std::string& src, const std::string tok, OutputIterator outit, bool bIgnoreEmptyItem	= true) {
    if(	src.empty()	) {
        return;
    }
    if(	tok.empty()	) {
        *outit	= src;
        ++outit;
        return;
    }

    std::string::size_type pre_index = 0, index	= 0, len = 0;
    while( (index =	src.find_first_of(tok, pre_index)) != std::string::npos	) {
        len = index - pre_index;
        if( !bIgnoreEmptyItem || len != 0 ) {
            *outit	= src.substr(pre_index,	len);
            ++outit;
        }
        pre_index =	index +	1;
    }

    *outit	= src.substr(pre_index);
    ++outit;
}

//
//	is number
//
bool	string_isnumber(const std::string& str);

//
//	tobool
//
bool	string_tobool(const std::string& str, bool& v);

//
//	tonumbers.
//
template<typename T1>
bool	string_tonumbers(const std::string& str, T1& v1) {
    std::istringstream is(str);

    return	(is	>> v1)?true:false;
}
template<typename T1, typename T2>
bool	string_tonumbers(const std::string& str, T1& v1, T2& v2) {
    std::deque<std::string>	strs;
    string_split(str, ",\t |", std::back_inserter(strs), true);

    if(strs.size() < 2)	return false;
    return	string_tonumbers(strs[0],v1) && string_tonumbers(strs[1],v2);
}
template<typename T1, typename T2, typename	T3>
bool	string_tonumbers(const std::string& str, T1& v1, T2& v2, T3& v3) {
    std::deque<std::string>	strs;
    string_split(str, ",\t |", std::back_inserter(strs), true);

    if(strs.size() < 3)	return false;
    return	string_tonumbers(strs[0],v1)	&&	string_tonumbers(strs[1],v2)	&&	string_tonumbers(strs[2],v3);
}
template<typename T1, typename T2, typename	T3,	typename T4>
bool	string_tonumbers(const std::string& str, T1& v1, T2& v2, T3& v3, T4& v4) {
    std::deque<std::string>	strs;
    string_split(str, ",\t |", std::back_inserter(strs), true);

    if(strs.size() < 4)	return false;
    return	string_tonumbers(strs[0],v1)	&&	string_tonumbers(strs[1],v2)
            &&	string_tonumbers(strs[2],v3)	&&	string_tonumbers(strs[3],v4)
            ;
}

//	find a substring in a string (case insensitive)
const char*		string_ifind(const char * str, const char * substr);
size_t			string_ifind(const std::string& str, const std::string& substr);

//
//	match wildcard, like dir command： * ?
//
bool			string_match_wildcard(const std::string& str, const char *pszMatch);
bool			string_match_wildcard(const char *pszString, const char *pszMatch);

//
//	replace
//
void			string_replace(std::string& str, const std::string& old_value, const std::string& new_value);
std::string		string_replace(const char* str, const std::string& old_value, const std::string& new_value);
void			string_replace_recursive(std::string& str, const std::string& old_value, const std::string& new_value);
std::string		string_replace_recursive(const char* str, const std::string& old_value, const std::string& new_value);
int				string_ireplace(const char* lpszStr, const char* lpszOld, const char* lpszNew, char* lpszResult);
void			string_ireplace(std::string& str, const std::string& old_value, const std::string& new_value);

//
//	genreate randome string.
//
std::string		string_generate(size_t length, bool use_special_chars);

//
//	ellipsize operations.
//
std::string		string_ellipsis(const char* str, size_t nShowLen);
void			string_ellipsis(std::string& str, size_t nShowLen);

//
//	html operations.
//
size_t				string_html_strip_tag(char *str);
const std::string&	string_html_strip_tag(std::string& str);
int					string_html_to_text(const char *html, char *plain,  size_t dwPlainSize);
std::string			string_html_to_text(const std::string& html);

bool                string_parse_url(const char* url,
                                     std::string *scheme,
                                     std::string *host,
                                     std::string *port,
                                     std::string *path,
                                     std::string *query,
                                     std::string *fragment,
                                     std::string *username,
                                     std::string *password);

//
//	wchar <-> utf8 convert
//
size_t				string_wchar_to_utf8(const wchar_t* src, size_t src_byte, char* dest, size_t dest_byte);
size_t				string_utf8_to_wchar(const char* src, size_t src_byte, wchar_t* dest, size_t dest_byte);

//
//	!Note:
//	1.	字符串最大长度为 STRING_CONVERSION_MAX_SIZE
//	2.	非windows平台，统一认为 Ansi == UTF-8
//

#ifndef		STRING_CONVERSION_MAX_SIZE
#	define	STRING_CONVERSION_MAX_SIZE	1024 * 32
#endif

const char*				string_utf8_to_ansi(const std::string& input, size_t*	output_size = 0);
const char*				string_ansi_to_utf8(const std::string& input, size_t*	output_size = 0);

bool					string_utf8_to_ansi(const std::string& input, std::string& output);
bool					string_ansi_to_utf8(const std::string& input, std::string& output);

const char*				string_wchar_to_ansi(const std::wstring& input,size_t*	output_size = 0);
const wchar_t*			string_ansi_to_wchar(const std::string& input, size_t*	output_size = 0);

bool					string_wchar_to_ansi(const std::wstring& input,std::string& output);
bool					string_ansi_to_wchar(const std::string& input, std::wstring& output);

#if		defined(WIN32)
const char*				string_utf16_to_ansi(const std::wstring& input, size_t*	output_size = 0);
const unsigned short*	string_ansi_to_utf16(const std::string& input, size_t*	output_size = 0);

bool					string_utf16_to_ansi(const std::wstring& input, std::string& output);
bool					string_ansi_to_utf16(const std::string& input, std::wstring& output);
#endif

//
//	string line operations.
//
std::string			string_line_escape(const std::string& str);		//escape \\ \n \r
std::string			string_line_unescape(const std::string& str);	//unescape \\ \n \r

//
//	string json operations.
//
std::string			string_json_escape(const std::string& str);		//escape \\ \n \r ...
std::string			string_json_unescape(const std::string& str);	//unescape \\ \n \r ...

//
//	string xml operations.
//
std::string			string_xml_escape(const std::string& str);		//escape < > & ' "
std::string			string_xml_unescape(const std::string& str);	//unescape < > & ' "

//
//	string base64 operations.
//
bool				string_is_base64(const std::string& str);
std::string			string_base64_encode(const unsigned char* s, unsigned long len);
std::string			string_base64_decode(const unsigned char* s, unsigned long len);

inline	std::string	string_base64_encode(const char* s, unsigned int len) {
    return	string_base64_encode((const unsigned char*)s, (unsigned long)len);
}
inline	std::string	string_base64_encode(const std::string& s) {
    return	string_base64_encode((const unsigned char*)s.c_str(), (unsigned long)s.size());
}

inline  std::string	string_base64_decode(const std::string& s) {
    return  string_base64_decode((const unsigned char*)s.c_str(), (unsigned long)s.size());
}

//
//  string quoted-printable operations.
//
std::string			string_qp_encode(const unsigned char* s, unsigned long len);
std::string			string_qp_decode(const unsigned char* s, unsigned long len);

inline	std::string	string_qp_encode(const char* s, unsigned int len) {
    return	string_qp_encode((const unsigned char*)s, (unsigned long)len);
}
inline	std::string	string_qp_encode(const std::string& s) {
    return	string_qp_encode((const unsigned char*)s.c_str(), (unsigned long)s.size());
}
inline  std::string	string_qp_decode(const std::string& s) {
    return  string_qp_decode((const unsigned char*)s.c_str(), (unsigned long)s.size());
}

//
//	string	traits.
//
struct string_iless : public std::binary_function<const std::string&, const std::string&, bool>	{
    bool    operator()(const std::string& l, const std::string& r)const;
};

//
//  for istream_iterator and ostream_iterator
//
struct string_line {
    std::string data;
    operator    std::string() {
        return  data;
    }
};
inline  std::istream&   operator>>(std::istream& is, string_line& line) {
    std::getline(is, line.data);
    return  is;
}
inline  std::ostream&   operator<<(std::ostream& os, const string_line& line) {
    return  os << line.data;
}
inline  const char*     cstr(const string_line& line) {
    return  line.data.c_str();
}

#endif
