#include "string.h"

#include	<cassert>
#include	<cstdio>
#include	<cstring>
#include	<memory>

#if	!defined(UNI_REPLACEMENT_CHAR)
#	include	"_third/ConvertUTF.h"
#endif

#if	defined(_WIN32)
#	include	<windows.h>
#endif

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
