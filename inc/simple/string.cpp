#include "string.h"

#include	<cassert>
#include    <cstdarg>
#include	<cstdio>
#include	<cstring>
#include	<vector>
#include	<memory>

#include	"third/ConvertUTF.h"

#if	defined(_WIN32)
#	include	<windows.h>
#endif

#if	defined(_MSC_VER)
#	define	vsnprintf					_vsnprintf
#	define	vscprintf(buf,len,fmt,args)	_vscprintf(fmt, args)
#	define	stricmp						_stricmp
#	define	strnicmp					_strnicmp
#else
#	define	vscprintf					vsnprintf
#	define	strnicmp					strncasecmp
#	define	stricmp						strcasecmp
#endif

void	string_format(std::string& str, const char* fmt, ...) {
    str.clear();
    if (0 == fmt) {
        return;
    }

    va_list args;
    va_start(args, fmt);								//初始化变量参数
    int nLength	= vscprintf(0, 0, fmt, args) + 1;       //获取格式化字符串长度
    std::auto_ptr<char> buf(new char[nLength]);         //创建用于存储格式化字符串的字符数组
    va_end(args);

    va_start(args, fmt);								//重新初始化变量参数
    int	nRealLen	= vsnprintf(buf.get(), nLength, fmt, args);
    assert(nRealLen < nLength);
    if(nRealLen > 0) {
        str.assign(buf.get(), nRealLen);
    }
    va_end(args);
}

std::string		string_format(const char* fmt, ...) {
    std::string str;
    if (0 == fmt) {
        return str;
    }

    va_list args;
    va_start(args, fmt);								//初始化变量参数
    int nLength	= vscprintf(0, 0, fmt, args) + 1;       //获取格式化字符串长度
    std::auto_ptr<char> buf(new char[nLength]);         //创建用于存储格式化字符串的字符数组
    va_end(args);

    va_start(args, fmt);								//重新初始化变量参数
    int	nRealLen	= vsnprintf(buf.get(), nLength, fmt, args);
    assert(nRealLen < nLength);
    if(nRealLen > 0) {
        str.assign(buf.get(), nRealLen);
    }
    va_end(args);
    return str;
}

bool	string_isnumber(const std::string& str) {
    if(str.empty()) {
        return	false;
    }
    char* p;
    strtod(str.c_str(), &p);
    return *p == 0;
}

bool	string_tobool(const std::string& str, bool& v) {
    if(str.empty()) {
        return	false;
    }
    static	const char*		S_BOOL_STRINGS[][2]	= {
        {"0",		"1"},
        {"F",		"T"},
        {"N",		"Y"},
        {"OFF",		"ON"},
        {"FALSE",	"TRUE"},
        {"NO",		"YES"},
    };

    std::string	sTrimed(str);
    string_trim(sTrimed);
    const char*	pStr	= sTrimed.c_str();
    size_t		i		= 0;
    const int	size	= sizeof(S_BOOL_STRINGS)/sizeof(S_BOOL_STRINGS[0]);
    for(; i < size; ++i) {
        if(0 == stricmp(pStr,	S_BOOL_STRINGS[i][0])) {
            v	= false;
            break;
        }
        if(0 == stricmp(pStr,	S_BOOL_STRINGS[i][1])) {
            v	= true;
            break;
        }
    }

    return	(i < size);
}

//
//	类似于dir命令的匹配方式： * ?
//
bool string_match_wildcard(const char *pszString, const char *pszMatch) {
    const char *mp  = NULL;
    const char *cp  = NULL;

    while (*pszString) {
        if (*pszMatch == ('*')) {
            if (!*++pszMatch)
                return true;
            mp = pszMatch;
            cp = pszString + 1;
        } else if (*pszMatch == ('?') || toupper(*pszMatch) == toupper(*pszString)) {
            pszMatch++;
            pszString++;
        } else if (!cp)
            return false;
        else {
            pszMatch = mp;
            pszString = cp++;
        }
    }

    while (*pszMatch == ('*'))
        pszMatch++;

    return !*pszMatch;
}
bool	string_match_wildcard(const std::string& str, const char *pszMatch) {
    return	string_match_wildcard(str.c_str(), pszMatch);
}

void	string_replace(std::string& str, const std::string& old_value, const std::string& new_value) {
    for(std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        pos = str.find(old_value,pos);
        if(std::string::npos == pos) {
            break;
        }

        str.replace(pos,old_value.length(),new_value);
    }
}

void	string_replace_recursive(std::string& str, const std::string& old_value, const std::string& new_value) {
    while(true) {
        std::string::size_type   pos = str.find(old_value);
        if(std::string::npos == pos) {
            break;
        }

        str.replace(pos,old_value.length(),new_value);
    }
}

const char* string_ifind(const char * str, const char * substr) {
    if (!str || !substr || (substr[0] == '\0'))
        return str;

    size_t nLen = strlen(substr);
    while (*str) {
        if (strnicmp(str, substr, nLen) == 0)
            break;
        str++;
    }

    if (*str == '\0') {
        str = 0;
    }

    return str;
}

size_t string_ifind(const std::string& str, const std::string& substr) {
    const char* pFound	= string_ifind(str.c_str(), substr.c_str());
    return	(0 == pFound) ? std::string::npos : pFound - str.c_str();
}

int	string_ireplace(const char* lpszStr, const char* lpszOld, const char* lpszNew, char* lpszResult) {
    if (!lpszStr || !lpszOld || !lpszNew)
        return 0;

    size_t nStrLen = strlen(lpszStr);
    if (nStrLen == 0)
        return 0;

    size_t nOldLen = strlen(lpszOld);
    if (nOldLen == 0)
        return 0;

    size_t nNewLen = strlen(lpszNew);

    // loop once to figure out the size of the result string
    int nCount = 0;
    char *pszStart	= (char *) lpszStr;
    char *pszEnd	= (char *) lpszStr + nStrLen;
    char *pszTarget	= 0;
    char *pszResultStr = 0;

    while (pszStart < pszEnd) {
        while ((pszTarget = (char*)string_ifind(pszStart, lpszOld)) != NULL) {
            nCount++;
            pszStart = pszTarget + nOldLen;
        }
        pszStart += strlen(pszStart);
    }

    // if any changes, make them now
    if (nCount > 0) {
        // allocate buffer for result string
        size_t nResultStrSize = nStrLen + (nNewLen - nOldLen) * nCount + 2;
        pszResultStr = new char [nResultStrSize];
        memset(pszResultStr, 0, nResultStrSize*sizeof(char));

        pszStart = (char *) lpszStr;
        pszEnd = (char *) lpszStr + nStrLen;
        char *cp = pszResultStr;

        // loop again to actually do the work
        while (pszStart < pszEnd) {
            while ((pszTarget = (char*)string_ifind(pszStart, lpszOld)) != NULL) {
                int nCopyLen = (int)(pszTarget - pszStart);
                strncpy(cp, &lpszStr[pszStart-lpszStr], nCopyLen);

                cp += nCopyLen;

                pszStart = pszTarget + nOldLen;

                strcpy(cp, lpszNew);

                cp += nNewLen;
            }
            strcpy(cp, pszStart);
            pszStart += strlen(pszStart);
        }
        if (lpszResult && pszResultStr)
            strcpy(lpszResult, pszResultStr);
    }

    int nSize = 0;
    if (pszResultStr) {
        nSize = (int)strlen(pszResultStr);
        delete [] pszResultStr;
    }
    return nSize;
}

void	string_ireplace(std::string& str, const std::string& old_value, const std::string& new_value) {
    int	nRet	= string_ireplace(str.c_str(), old_value.c_str(), new_value.c_str(), 0);
    if(0 == nRet) {
        //	not found.
        return;
    }

    char*	pbuf	= new char[nRet + 1];
    memset(pbuf, 0, nRet + 1);
    string_ireplace(str.c_str(), old_value.c_str(), new_value.c_str(), pbuf);
    str.assign(pbuf, nRet);
    delete[]pbuf;
}

extern	"C"	int GeneratePassword(   int length,
                                    int special,
                                    char *password);
std::string		string_generate(size_t length, bool use_special_chars) {
    std::auto_ptr<char>	buf(new char[length]);
    if(0 == GeneratePassword(int(length), use_special_chars, buf.get())) {
        return	std::string(buf.get(), length);
    }

    std::string	str;
    str.reserve(length);
    for(size_t i = 0; i < length; ++i) {
        str.push_back('a' + rand()%26);
    }
    return	str;
}

std::string	string_replace(const char* str, const std::string& old_value, const std::string& new_value) {
    std::string	s(str);
    string_replace(s, old_value, new_value);
    return	s;
}

std::string	string_replace_recursive(const char* str, const std::string& old_value, const std::string& new_value) {
    std::string	s(str);
    string_replace_recursive(s, old_value, new_value);
    return	s;
}

size_t	string_wchar_to_utf8(const wchar_t* src, size_t src_byte, char* dest, size_t dest_byte) {
    size_t	widesize	= src_byte / sizeof(wchar_t);
    size_t	utf8size	= dest_byte;
    if (sizeof(wchar_t) == 2) {
        const UTF16* sourcestart	= reinterpret_cast<const UTF16*>(src);
        const UTF16* sourceend		= sourcestart + widesize;
        UTF8* targetstart			= reinterpret_cast<UTF8*>(dest);
        UTF8* targetend				= targetstart + utf8size;
        ConversionResult res		= ConvertUTF16toUTF8(&sourcestart, sourceend, &targetstart, targetend, lenientConversion);
        if (res != conversionOK) {
            return	0;
        }
        *targetstart = 0;
        return	(char*)targetstart - dest;
    } else if (sizeof(wchar_t) == 4) {
        const UTF32* sourcestart	= reinterpret_cast<const UTF32*>(src);
        const UTF32* sourceend		= sourcestart + widesize;
        UTF8* targetstart			= reinterpret_cast<UTF8*>(dest);
        UTF8* targetend				= targetstart + utf8size;
        ConversionResult res		= ConvertUTF32toUTF8(&sourcestart, sourceend, &targetstart, targetend, lenientConversion);
        if (res != conversionOK) {
            return	0;
        }
        *targetstart = 0;
        return	(char*)targetstart - dest;
    }

    return	0;
}

size_t	string_utf8_to_wchar(const char* src, size_t src_byte, wchar_t* dest, size_t dest_byte) {
    size_t utf8size = src_byte;
    size_t widesize = dest_byte / sizeof(wchar_t);
    if (sizeof(wchar_t) == 2) {
        const UTF8* sourcestart	= reinterpret_cast<const UTF8*>(src);
        const UTF8* sourceend	= sourcestart + utf8size;
        UTF16* targetstart		= reinterpret_cast<UTF16*>(dest);
        UTF16* targetend		= targetstart + widesize;
        ConversionResult res	= ConvertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, lenientConversion);
        if (res != conversionOK) {
            return	0;
        }
        *targetstart = 0;
        return (char*)targetstart - (char*)dest;
    } else if (sizeof(wchar_t) == 4) {
        const UTF8* sourcestart = reinterpret_cast<const UTF8*>(src);
        const UTF8* sourceend	= sourcestart + utf8size;
        UTF32* targetstart		= reinterpret_cast<UTF32*>(dest);
        UTF32* targetend		= targetstart + widesize;
        ConversionResult res	= ConvertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, lenientConversion);
        if (res != conversionOK) {
            return	0;
        }
        *targetstart = 0;
        return (char*)targetstart - (char*)dest;
    }

    return	0;
}

std::string	string_ellipsis(const char* str, size_t nShowLen) {
    if(0 == str) {
        return	std::string();
    }
    std::string	s(str);
    string_ellipsis(s, nShowLen);
    return	s;
}

void	string_ellipsis(std::string& str, size_t nShowLen) {
    size_t nSrcLen = str.size();
    if (nShowLen >= nSrcLen || nShowLen <= 3) {
        return;
    }

    size_t  wlen    = nSrcLen+1;
    std::auto_ptr<wchar_t>  wbuf(new wchar_t[wlen]);
    memset(wbuf.get(), 0, sizeof(wchar_t)*wlen);

    size_t  cvt_ret = 0;
#if	defined(_WIN32)
    cvt_ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), nSrcLen, wbuf.get(), wlen);
#else
    cvt_ret = string_utf8_to_wchar(str.c_str(), nSrcLen, wbuf.get(), sizeof(wchar_t) * wlen) / sizeof(wchar_t);
#endif

    if(0 == cvt_ret) {
        return;
    }

    size_t      wRealLen    = 0;
    wchar_t*    pwbuf       = wbuf.get();
    for (size_t i = 0, nLen = 0; i < nSrcLen; ++i) {
        if (pwbuf[i] == 0x00)			break;
        else if (pwbuf[i] >= 0x80)		nLen += 2;
        else							nLen++;

        if (nLen >= nShowLen-2) {
            size_t k = i;
            if (pwbuf[k+1] == '\0')
                break;

            if (nLen == nShowLen-1 && pwbuf[k+1] <= 0x80)
                --k;

            pwbuf[k+1]   = pwbuf[k+2] = '.';
            pwbuf[k+3]   = '\0';
            wRealLen    = k + 2;
            break;
        }
    }

    std::auto_ptr<char> dbuf(new char[nSrcLen + 1]);
    memset(dbuf.get(), 0, sizeof(char) * (nSrcLen + 1));

#if	defined(_WIN32)
    cvt_ret = WideCharToMultiByte (CP_ACP, 0, pwbuf, wRealLen + 1, dbuf.get(),  nSrcLen + 1, NULL, NULL);
#else
    cvt_ret = string_wchar_to_utf8(pwbuf, (wRealLen + 1) * sizeof(wchar_t), dbuf.get(), nSrcLen + 1);
#endif

    if(0 == cvt_ret) {
        return;
    }

    str.assign(dbuf.get());
    return;
}

size_t	string_html_strip_tag(char *s) {
    int i, j, tag = 0;
    i = j = 0;
    while (s[i]) {
        if (s[i] == '<')	tag = 1;
        if (!tag)			s[j++] = s[i];
        if (s[i] == '>')	tag = 0;
        i++;
    }
    s[j] = '\0';
    return	size_t(j);
}

const std::string&	string_html_strip_tag(std::string& str) {
    char*	s	= new char[str.size() + 1];
    memcpy(s, str.c_str(), str.size() + 1);
    str.assign(s, string_html_strip_tag(s));
    return	str;
}

//
//	替换 html 字符串 为 纯文本字符串
//
int string_html_to_text(const char *html, char *plain,  size_t dwPlainSize) {
    struct CHAR_ENTITIES {
        const char *	pszName;		// string entered in HTML - e.g., "&nbsp;"
        char			cCode;			// code generated by XHtmlDraw
        char			cSymbol;		// character symbol displayed
    };

    static const CHAR_ENTITIES m_aCharEntities[] = {
        { "&amp;",		0,	'&'		},	// ampersand
        { "&bull;",		0,	'\x95'	},	// bullet      NOT IN MS SANS SERIF
        { "&cent;",		0,	'\xA2'	},	// cent sign
        { "&copy;",		0,	'\xA9'	},	// copyright
        { "&deg;",		0,	'\xB0'	},	// degree sign
        { "&euro;",		0,	'\x80'	},	// euro sign
        { "&frac12;",	0,	'\xBD'	},	// fraction one half
        { "&frac14;",	0,	'\xBC'	},	// fraction one quarter
        { "&gt;",		0,	'>'		},	// greater than
        { "&iquest;",	0,	'\xBF'	},	// inverted question mark
        { "&lt;",		0,	'<'		},	// less than
        { "&micro;",	0,	'\xB5'	},	// micro sign
        { "&middot;",	0,	'\xB7'	},	// middle dot = Georgian comma
        { "&nbsp;",		0,	' '		},	// nonbreaking space
        { "&para;",		0,	'\xB6'	},	// pilcrow sign = paragraph sign
        { "&plusmn;",	0,	'\xB1'	},	// plus-minus sign
        { "&pound;",	0,	'\xA3'	},	// pound sign
        { "&quot;",		0,	'"'		},	// quotation mark
        { "&reg;",		0,	'\xAE'	},	// registered trademark
        { "&sect;",		0,	'\xA7'	},	// section sign
        { "&sup1;",		0,	'\xB9'	},	// superscript one
        { "&sup2;",		0,	'\xB2'	},	// superscript two
        { "&times;",	0,	'\xD7'	},	// multiplication sign
        { "&trade;",	0,	'\x99'	},	// trademark   NOT IN MS SANS SERIF
        { 0,			0,	0		},	// MUST BE LAST
    };

    size_t nSize = 0;
    assert(0 != html);
    if (0 != html) {
        if (plain) {
            memset(plain, 0, dwPlainSize*sizeof(char));
        }
        if (strchr(html, ('<')) || strchr(html, ('&'))) {
            // there is html in this string

            // we are stripping out character entities, so the
            // resulting string will be shorter than the original
            size_t buflen = strlen(html) + 100;

            std::auto_ptr<char>	buf1(new char [buflen]);
            std::auto_ptr<char>	buf2(new char [buflen]);

            memset(buf1.get(), 0, buflen*sizeof(char));
            memset(buf2.get(), 0, buflen*sizeof(char));

            strncpy(buf1.get(), html, buflen-1);
            string_html_strip_tag(buf1.get());

            char ent[2] = { ('\0') };

            // loop to remove character entities
            for (int i = 0; m_aCharEntities[i].pszName != NULL; i++) {
                ent[0] = m_aCharEntities[i].cSymbol;
                int nRep = string_ireplace(buf1.get(), m_aCharEntities[i].pszName, ent, buf2.get());
                if (nRep > 0) {
                    strcpy(buf1.get(), buf2.get());
                }
            }
            if (plain) {
                strncpy(plain, buf1.get(), dwPlainSize-1);
                nSize = strlen(plain);
            } else {
                // no output buffer, just return required size
                nSize = strlen(buf1.get());
            }
        } else {
            // no html in string, just copy to output buffer
            if (plain) {
                strncpy(plain, html, dwPlainSize-1);
                nSize = strlen(plain);
            } else {
                // no output buffer, just return required size
                nSize = strlen(html);
            }
        }
    }

    return (int)nSize;
}

std::string	string_html_to_text(const std::string& html) {
    int	size	= string_html_to_text(html.c_str(), 0, 0);
    std::auto_ptr<char>	pbuf(new char[size + 1]);
    memset(pbuf.get(), 0, size + 1);
    string_html_to_text(html.c_str(), pbuf.get(), size + 1);
    return	std::string(pbuf.get(), size_t(size));
}

std::string    string_line_escape(const std::string& str) {
    std::string  ret;
    ret.reserve(size_t(str.size() * 1.5));
    for(size_t i = 0; i < str.size(); ++i) {
        switch(str[i]) {
        case '\n':
            ret.push_back('\\');
            ret.push_back('n');
            break;
        case '\r':
            ret.push_back('\\');
            ret.push_back('r');
            break;
        case '\\':
            ret.push_back('\\');
            ret.push_back('\\');
            break;
        default:
            ret.push_back(str[i]);
        }
    }
    return ret;
}

std::string    string_line_unescape(const std::string& str) {
    std::string  ret;
    ret.reserve(str.size());
    for(size_t i = 0; i < str.size(); ++i) {
        if('\\' == str[i]) {
            ++i;
            switch(str[i]) {
            case 'n':
                ret.push_back('\n');
                break;
            case 'r':
                ret.push_back('\r');
                break;
            case '\\':
                ret.push_back('\\');
                break;
            default:
                ret.push_back('\\');
                ret.push_back(str[i]);
                break;
            }
        } else {
            ret.push_back(str[i]);
        }
    }
    return ret;
}

std::string		string_json_escape(const std::string& str) {
    std::string  ret;
    ret.reserve(size_t(str.size() * 1.5));
    for(size_t i = 0; i < str.size(); ++i) {
        switch(str[i]) {
        case '\t':
            ret	+= "\\t";
            break;
        case '"':
            ret	+= "\\\"";
            break;
        case '\\':
            ret	+= "\\\\";
            break;
        case '\n':
            ret	+= "\\n";
            break;
        case '\r':
            ret	+= "\\r";
            break;
        case '/':
            ret	+= "\\/";
            break;
        case '\b':
            ret	+= "\\b";
            break;
        case '\f':
            ret	+= "\\f";
            break;
        default:
            ret.push_back(str[i]);
            break;
        }
    }
    return ret;
}

std::string		string_json_unescape(const std::string& str) {
    std::string  ret;
    ret.reserve(str.size());
    for(size_t i = 0; i < str.size(); ++i) {
        if('\\' != str[i]) {
            ret.push_back(str[i]);
            continue;
        }
        switch(str[++i]) {
        case 't':
            ret.push_back('\t');
            break;
        case '"':
            ret.push_back('"');
            break;
        case '\\':
            ret.push_back('\\');
            break;
        case 'n':
            ret.push_back('\n');
            break;
        case 'r':
            ret.push_back('\r');
            break;
        case '/':
            ret.push_back('/');
            break;
        case '\b':
            ret.push_back('\b');
            break;
        case '\f':
            ret.push_back('\f');
            break;
        default:
            ret.push_back('\\');
            ret.push_back(str[i]);
            break;
        }
    }
    return ret;
}

std::string	string_xml_escape(const std::string& str) {
    std::string  ret;
    ret.reserve(size_t(str.size() * 1.5));
    for(size_t i = 0; i < str.size(); ++i) {
        switch(str[i]) {
        case '<':
            ret	+= "&lt;";
            break;
        case '>':
            ret	+= "&gt;";
            break;
        case '&':
            ret	+= "&amp;";
            break;
        case '\'':
            ret	+= "&apos;";
            break;
        case '"':
            ret	+= "&quot;";
            break;
        default:
            ret.push_back(str[i]);
            break;
        }
    }
    return ret;
}

std::string	string_xml_unescape(const std::string& str) {
    std::string  ret;
    ret.reserve(str.size());
    for(size_t i = 0; i < str.size(); ++i) {
        if('&' != str[i]) {
            ret.push_back(str[i]);
            continue;
        }
        switch(str[i+1]) {
        case 'l':
            if(0 == strncmp(str.c_str() + i, "&lt;", 4)) {
                ret.push_back('<');
                i	+= 3;
                continue;
            }
            break;
        case 'g':
            if(0 == strncmp(str.c_str() + i, "&gt;", 4)) {
                ret.push_back('>');
                i	+= 3;
                continue;
            }
            break;
        case 'a':
            if(0 == strncmp(str.c_str() + i, "&amp;", 5)) {
                ret.push_back('&');
                i	+= 4;
                continue;
            }
            if(0 == strncmp(str.c_str() + i, "&apos;", 6)) {
                ret.push_back('\'');
                i	+= 5;
                continue;
            }
            break;
        case 'q':
            if(0 == strncmp(str.c_str() + i, "&quot;", 6)) {
                ret.push_back('"');
                i	+= 5;
                continue;
            }
            break;
        }
        ret.push_back(str[i]);
    }
    return ret;
}

bool	string_iless::operator()(const std::string& l, const std::string& r)const {
    return	(l == r) ? false : (stricmp(l.c_str(), r.c_str()) < 0);
}
