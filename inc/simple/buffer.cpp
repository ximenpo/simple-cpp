
#include <cassert>
#include <climits>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <memory>

#if	defined(_WIN32)
#	include <windows.h>
#endif

#include "byte.h"

#include "buffer.h"
#include "string.h"

buffer_tag::SIZE_TAG	buffer_size_tag(int32_t value) {
    return	(0 == value)?									buffer_tag::TAG_0
            :	(INT8_MIN <= value && value <= INT8_MAX)?	buffer_tag::TAG_1
            :	(INT16_MIN <= value && value <= INT16_MAX)?	buffer_tag::TAG_2
            :	buffer_tag::TAG_4
            ;
}
buffer_tag::SIZE_TAG	buffer_size_tag(uint32_t value) {
    return	(0 == value)?				buffer_tag::TAG_0
            :	(value <= UINT8_MAX)?	buffer_tag::TAG_1
            :	(value <= UINT16_MAX)?	buffer_tag::TAG_2
            :	buffer_tag::TAG_4
            ;
}
buffer_tag::SIZE_TAG	buffer_size_tag(int64_t value) {
    return	(0 == value)?									buffer_tag::TAG_0
            :	(INT8_MIN <= value && value <= INT8_MAX)?	buffer_tag::TAG_1
            :	(INT16_MIN <= value && value <= INT16_MAX)?	buffer_tag::TAG_2
            :	(INT32_MIN <= value && value <= INT16_MAX)?	buffer_tag::TAG_4
            :	buffer_tag::TAG_8
            ;
}
buffer_tag::SIZE_TAG	buffer_size_tag(uint64_t value) {
    return	(0 == value)?				buffer_tag::TAG_0
            :	(value <= UINT8_MAX)?	buffer_tag::TAG_1
            :	(value <= UINT16_MAX)?	buffer_tag::TAG_2
            :	(value <= UINT32_MAX)?	buffer_tag::TAG_4
            :	buffer_tag::TAG_8
            ;
}
buffer_tag::SIZE_TAG	buffer_size_tag(float value) {
    return	buffer_tag::TAG_4;
}
buffer_tag::SIZE_TAG	buffer_size_tag(double value) {
    return	buffer_tag::TAG_8;
}
buffer_tag::SIZE_TAG	buffer_size_tag(bool value) {
    return	value?	buffer_tag::TAG_1	:	buffer_tag::TAG_0;
}
#if defined(_WIN64) ||  defined(__x86_64__)
buffer_tag::SIZE_TAG	buffer_size_tag(size_t value) {
    return	buffer_size_tag(uint64_t(value));
}
#endif

bool	buffer_write_tag(buffer& buf, const buffer_tag& tag) {
    uint8_t	flag	= tag.pack();
    return	buf.write(&flag, sizeof(flag));
}

bool	buffer_read_tag(buffer& buf, buffer_tag& tag) {
    uint8_t	type_flag;
    if(!buf.read(&type_flag, sizeof(type_flag))) {
        return	false;
    }

    tag.unpack(type_flag);
    return	true;
}

bool	buffer_write_uint_value(buffer& buf, uint8_t size_tag, uintmax_t value) {
    switch(size_tag) {
    case buffer_tag::TAG_0: {
        return	buf.good();
    }
    break;
    case buffer_tag::TAG_1: {
        uint8_t	tmp_v	= uint8_t(value);
        return	buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_2: {
        uint16_t	tmp_v	= byte_hton_2(uint16_t(value));
        return	buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_4: {
        uint32_t	tmp_v	= byte_hton_4(uint32_t(value));
        return	buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_8: {
        uint64_t	tmp_v	= byte_hton_8(uint64_t(value));
        return	buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    }

    return	false;
}

bool	buffer_read_uint_value(buffer& buf, uint8_t size_tag, uintmax_t& value) {
    switch(size_tag) {
    case buffer_tag::TAG_0:
        value = 0;
        break;
    case buffer_tag::TAG_1: {
        uint8_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            value	= tmp_v;
        }
    }
    break;
    case buffer_tag::TAG_2: {
        uint16_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_2(tmp_v);
            value	= tmp_v;
        }
    }
    break;
    case buffer_tag::TAG_4: {
        uint32_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_4(tmp_v);
            value	= tmp_v;
        }
    }
    break;
    case buffer_tag::TAG_8: {
        uint64_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_8(tmp_v);
            value	= tmp_v;
        }
    }
    break;
    default: {
        value   = 0;
        return	false;
    }
    break;
    }

    return	true;
}

bool	buffer_write_raw(buffer& buf, const void* pData, size_t nLen) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_RAW,
        buffer_size_tag(nLen),
        false
    };

    //	begin tag.
    if(		!buffer_write_tag(buf, tag)
            ||	!buffer_write_uint_value(buf, tag.size_tag, nLen)
      ) {
        return	false;
    }

    return	buf.write(pData, nLen);
}

bool	buffer_read_raw(buffer& buf, void* pData, size_t& nLen) {
    uintmax_t	size;
    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_RAW
            ||	!buffer_read_uint_value(buf, tag.size_tag, size)
            ||	size > nLen
      ) {
        buf.set_failure();
        return	false;
    }

    nLen	= size;

    return	buf.read(pData, size);
}

bool	buffer_read_and_ignore(buffer& buf) {
    uintmax_t	size;
    buffer_tag	tag;
    if(!buffer_read_tag(buf, tag)) {
        return	false;
    }

    uintmax_t	todo_bypes	= 0;
    switch(tag.data_type) {
    case buffer_tag::TYPE_NONE:
        break;	// nothing to read.
    case buffer_tag::TYPE_BOOL:
        break;	// nothing to read.
    case buffer_tag::TYPE_RAW:
    case buffer_tag::TYPE_STRING: {
        // string & raw read len, and read data.
        if(!buffer_read_uint_value(buf, tag.size_tag, todo_bypes)) {
            return	false;
        }
    }
    break;
    case buffer_tag::TYPE_INT:
    case buffer_tag::TYPE_UINT: {
        // int & uint read direct data.
        switch(tag.size_tag) {
        case buffer_tag::TAG_0:
            break;
        case buffer_tag::TAG_1:
            todo_bypes	= 1;
            break;
        case buffer_tag::TAG_2:
            todo_bypes	= 2;
            break;
        case buffer_tag::TAG_4:
            todo_bypes	= 4;
            break;
        case buffer_tag::TAG_8:
            todo_bypes	= 8;
            break;
        default:
            return	false;
        }
    }
    break;
    case buffer_tag::TYPE_REAL: {
        // float & double read 4/8 data.
        switch(tag.size_tag) {
        case buffer_tag::TAG_0:
            break;
        case buffer_tag::TAG_4:
            todo_bypes	= 4;
            break;
        case buffer_tag::TAG_8:
            todo_bypes	= 8;
            break;
        default:
            return	false;
        }
    }
    break;
    case buffer_tag::TYPE_ARRAY:
    case buffer_tag::TYPE_OBJECT: {
        // array & object -> read size, version, and contents.
        if(!buffer_read_uint_value(buf, tag.size_tag, size)) {
            return	false;
        }

        uint8_t	version	= 0;
        if(tag.version_tag) {
            buf.read(&version, 1);
        }

        if(tag.data_type == buffer_tag::TYPE_OBJECT) {
            for(size_t i = 0; i < size && buf.good(); ++i) {
                if(!buffer_read_and_ignore(buf)) {
                    return	false;
                }
            }

            break;
        }

        assert(tag.data_type == buffer_tag::TYPE_ARRAY);

        for(size_t i = 0; i < size && buf.good(); ++i) {
            for(size_t j = 0; j < size && buf.good(); ++j) {
                if(!buffer_read_and_ignore(buf)) {
                    return	false;
                }
            }
        }
    }
    break;
    default: {
        return	false;
    }
    }

    return	buf.read(0, todo_bypes);
}

static	const	size_t      GC_SERIALIZE_STRING_MAX_SIZE	= 4096;
static	inline	uintmax_t   MinValue(uintmax_t l, uintmax_t r) {
    return	l <= r ? l : r;
}


buffer::buffer(size_t nMaxSize)
    :	buf_owner_(true)
    ,	failure_(false)
    ,	data_(0)
    ,	capacity_(0)
    ,	size_(0)
    ,	offset_(0) {
    data_	    = new	uint8_t[nMaxSize];
    capacity_   = nMaxSize;
}

buffer::buffer(const void* pBuf, size_t nLen)
    :	buf_owner_(false)
    ,	failure_(false)
    ,	data_((uint8_t*)pBuf)
    ,	capacity_(nLen)
    ,	size_(nLen)
    ,	offset_(0) {
    assert(0 != pBuf);
}

buffer::~buffer() {
    if(buf_owner_) {
        delete	data_;
    }
}

std::string buffer::dump() {
    const size_t	NUMBER_SUM_PER_LINE	= 16;

    size_t			size	= this->size();
    const uint8_t*	pBuffer	= static_cast<const uint8_t*>(this->data());

    char buf[4]	= {0};
    std::string line;
    line.reserve(1024 * 4);
    for(size_t i = 0; i < size; ++i) {
        if(i > 0 && i % NUMBER_SUM_PER_LINE == 0) {
            line	+= "\n";
        }
        string_hex((unsigned char)pBuffer[i], buf);
        line	+= buf;
    }

    return line;
}

std::ostream& buffer::dump(std::ostream& os) {
    const	size_t	NUMBER_SUM_PER_LINE	= 16;

    size_t			size	= this->size();
    const uint8_t*	pBuffer	= static_cast<const uint8_t*>(this->data());

    char buf[4]	= {0};
    std::string line;
    for(size_t i = 0; i < size; ++i) {
        if(i > 0 && i % NUMBER_SUM_PER_LINE == 0) {
            os	<<	"\n";
        }
        string_hex((unsigned char)pBuffer[i], buf);
        os	<<	buf;
    }

    return os;
}

bool buffer::write(const void* pData, size_t nLen) {
    assert(0 != pData);
    assert((offset_ + nLen) <= capacity_);

    if(failure()) {
        return	false;
    }

    if(offset_ + nLen > capacity_) {
        failure_	= true;
        return false;
    }

    memcpy(&data_[offset_], pData, nLen);
    offset_	+= nLen;
    if(offset_ > size_) {
        size_	= offset_;
    }
    return true;
}

bool	buffer::read(void* pData, size_t nLen) {
    assert((size_ - offset_) >= nLen);

    if(failure()) {
        return	false;
    }

    if(size_ - offset_ < nLen) {
        failure_	= true;
        return false;
    }

    if(0 != pData) {
        memcpy(pData, &data_[offset_], nLen);
    }
    offset_	+= nLen;
    return true;
}

buffer& operator<<(buffer& buf, int8_t value) {
    return	buf	<< int64_t(value);
}
buffer& operator<<(buffer& buf, int16_t value) {
    return	buf	<< int64_t(value);
}
buffer& operator<<(buffer& buf, int32_t value) {
    return	buf	<< int64_t(value);
}
buffer& operator<<(buffer& buf, int64_t value) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_INT,
        buffer_size_tag(value),
        false
    };

    buffer_write_tag(buf, tag);

    switch(tag.size_tag) {
    case buffer_tag::TAG_0:
        break;
    case buffer_tag::TAG_1: {
        uint8_t	tmp_v	= uint8_t(value);
        buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_2: {
        uint16_t	tmp_v	= byte_hton_2(uint16_t(value));
        buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_4: {
        uint32_t	tmp_v	= byte_hton_4(uint32_t(value));
        buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    case buffer_tag::TAG_8: {
        uint64_t	tmp_v	= byte_hton_8(uint64_t(value));
        buf.write(&tmp_v, sizeof(tmp_v));
    }
    break;
    }

    return	buf;
}

buffer& operator>>(buffer& buf, int8_t& value) {
    value	= 0;

    int64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_1) {
            value	= int8_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, int16_t& value) {
    value	= 0;

    int64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_2) {
            value	= int16_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, int32_t& value) {
    value	= 0;

    int64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_4) {
            value	= int32_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, int64_t& value) {
    value	= 0;

    if(buf.failure()) {
        return	buf;
    }

    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_INT
      ) {
        buf.set_failure();
        return	buf;
    }

    switch(tag.size_tag) {
    case buffer_tag::TAG_0:
        break;
    case buffer_tag::TAG_1: {
        int8_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            value	= tmp_v;
        }
    }
    break;
    case buffer_tag::TAG_2: {
        int16_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_2(tmp_v);
            value	= tmp_v;
        }
    }
    break;
    case buffer_tag::TAG_4: {
        int32_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_4(tmp_v);
            value	= int(tmp_v);
        }
    }
    break;
    case buffer_tag::TAG_8: {
        int64_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= byte_ntoh_8(tmp_v);
            value	= int(tmp_v);
        }
    }
    break;
    default: {
        buf.set_failure();
    }
    break;
    }

    return	buf;
}

buffer& operator<<(buffer& buf, uint8_t value) {
    return	buf	<< uint64_t(value);
}
buffer& operator<<(buffer& buf, uint16_t value) {
    return	buf	<< uint64_t(value);
}
buffer& operator<<(buffer& buf, uint32_t value) {
    return	buf	<< uint64_t(value);
}
buffer& operator<<(buffer& buf, uint64_t value) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_UINT,
        buffer_size_tag(value),
        false
    };

    buffer_write_tag(buf, tag);
    buffer_write_uint_value(buf, tag.size_tag, value);

    return	buf;
}

buffer& operator>>(buffer& buf, uint8_t& value) {
    value	= 0;

    uint64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_1) {
            value	= uint8_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, uint16_t& value) {
    value	= 0;

    uint64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_2) {
            value	= uint8_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, uint32_t& value) {
    value	= 0;

    uint64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_4) {
            value	= uint8_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}

buffer& operator>>(buffer& buf, uint64_t& value) {
    value	= 0;

    if(buf.failure()) {
        return	buf;
    }

    uintmax_t	tmp_value;
    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_UINT
            ||	!buffer_read_uint_value(buf, tag.size_tag, tmp_value)
      ) {
        buf.set_failure();
        return	buf;
    }

    value	= uint64_t(tmp_value);
    return	buf;
}

#if defined(_WIN64) ||  defined(__x86_64__)
buffer&		operator<<(buffer& buf, size_t value) {
    return	buf << uint64_t(value);
}
buffer&		operator>>(buffer& buf, size_t& value) {
    value	= 0;

    uint64_t	tmp_v;
    buf	>> tmp_v;

    if(buf.good()) {
        if(buffer_size_tag(tmp_v) <= buffer_tag::TAG_4) {
            value	= uint8_t(tmp_v);
        } else {
            buf.set_failure();
        }
    }

    return	buf;
}
#endif

buffer& operator<<(buffer& buf, bool value) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_BOOL,
        value?	buffer_tag::TAG_1	:	buffer_tag::TAG_0
    };

    buffer_write_tag(buf, tag);

    return	buf;
}

buffer& operator>>(buffer& buf, bool& value) {
    value	= false;

    if(buf.failure()) {
        return	buf;
    }

    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_BOOL
      ) {
        buf.set_failure();
        return	buf;
    }

    switch(tag.size_tag) {
    case buffer_tag::TAG_0:
        value	= false;
        break;
    case buffer_tag::TAG_1:
        value	= true;
        break;
    default:
        buf.set_failure();
        break;
    }

    return	buf;
}

buffer& operator<<(buffer& buf, float value) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_REAL,
        buffer_tag::TAG_4
    };

    buffer_write_tag(buf, tag);

    {
        uint32_t	tmp_v	= htonl(*(uint32_t*)(&value));
        buf.write(&tmp_v, sizeof(tmp_v));
    }

    return	buf;
}

buffer& operator>>(buffer& buf, float& value) {
    value	= 0;

    if(buf.failure()) {
        return	buf;
    }

    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_REAL
            ||	tag.size_tag != buffer_tag::TAG_4
      ) {
        buf.set_failure();
        return	buf;
    }

    {
        uint32_t	tmp_v	= 0;
        if(buf.read(&tmp_v, sizeof(tmp_v))) {
            tmp_v	= ntohl(tmp_v);
            memcpy(&value, &tmp_v, sizeof(value));
        }
    }

    return	buf;
}

buffer& operator<<(buffer& buf, double value) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_REAL,
        buffer_tag::TAG_8
    };

    buffer_write_tag(buf, tag);

    long netnumber_1 = htonl(1);
    if(1==netnumber_1) {
        buf.write(&value, sizeof(value));
    } else {
        uint32_t	tmp_v	= htonl(*((uint32_t*)(&value) + 1));
        buf.write(&tmp_v, sizeof(tmp_v));
        tmp_v	= htonl(*((uint32_t*)(&value) + 0));
        buf.write(&tmp_v, sizeof(tmp_v));
    }

    return	buf;
}

buffer& operator>>(buffer& buf, double& value) {
    value	= 0;

    if(buf.failure()) {
        return	buf;
    }

    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	tag.data_type != buffer_tag::TYPE_REAL
            ||	tag.size_tag != buffer_tag::TAG_8
      ) {
        buf.set_failure();
        return	buf;
    }

    long netnumber_1 = htonl(1);
    if(1==netnumber_1) {
        buf.read(&value, sizeof(value));
    } else {
        uint8_t	tmp_v[sizeof(double)]	= {0};
        for(size_t i = sizeof(double); i > 0; --i) {
            buf.read(&tmp_v[i-1], 1);
        }

        if(buf.good()) {
            memcpy(&value, &tmp_v, sizeof(value));
        }
    }

    return	buf;
}

//
//	Win32 String	-> Ansi->UTF8
//	Other String	-> UTF8->UTF8
//
#if		defined(_WIN32)
static	bool	WriteSerialString(buffer& buf, const wchar_t* data, unsigned long size) {
    char			tmp_c[GC_SERIALIZE_STRING_MAX_SIZE*3];
    unsigned long	size_c	= WideCharToMultiByte (CP_UTF8, 0,
                              data, int(MinValue(size,GC_SERIALIZE_STRING_MAX_SIZE)),
                              tmp_c, int(sizeof(tmp_c)/sizeof(tmp_c[0])),
                              0, 0
                                               );

    buffer_tag	tag	= {
        buffer_tag::TYPE_STRING,
        buffer_size_tag(size_c),
        false
    };

    return	(	buffer_write_tag(buf, tag)
                &&		buffer_write_uint_value(buf, tag.size_tag, size_c)
                &&		buf.write(tmp_c, size_c)
           );
}

static	unsigned long	ReadSerialString(buffer& buf, wchar_t* data, unsigned long size) {
    unsigned long	size_c;
    char			tmp_c[GC_SERIALIZE_STRING_MAX_SIZE*3];

    buffer_tag	tag;
    if(		!buffer_read_tag(buf, tag)
            ||	!buffer_read_uint_value(buf, tag.size_tag, size_c)
            ||	size_c > GC_SERIALIZE_STRING_MAX_SIZE*3
            ||	!buf.read(tmp_c, size_c)
      ) {
        buf.set_failure();
        return	0;
    }

    unsigned long	size_w = MultiByteToWideChar (CP_UTF8, 0,
                             tmp_c, int(size_c),
                             data, int(size)
                                               );

    return	size_w;
}

buffer& operator<<(buffer& buf, const std::string& value) {
    wchar_t			tmp_v[GC_SERIALIZE_STRING_MAX_SIZE];
    unsigned long	size = MultiByteToWideChar (CP_ACP, 0,
                           value.c_str(), int(MinValue(value.size(), GC_SERIALIZE_STRING_MAX_SIZE)),
                           tmp_v, int(sizeof(tmp_v)/sizeof(tmp_v[0]))
                                             );

    WriteSerialString(buf, tmp_v, size);

    return	buf;
}
buffer& operator>>(buffer& buf, std::string& value) {
    char			tmp_c[GC_SERIALIZE_STRING_MAX_SIZE * 2];
    wchar_t			tmp_w[GC_SERIALIZE_STRING_MAX_SIZE];
    unsigned long	size_w	= ReadSerialString(buf, tmp_w, sizeof(tmp_w)/sizeof(tmp_w[0]));
    if(buf.good()) {
        unsigned long size_c	= WideCharToMultiByte (CP_ACP, 0,
                                  tmp_w, int(size_w),
                                  tmp_c, int(sizeof(tmp_c)/sizeof(tmp_c[0])),
                                  0, 0
                                                   );

        value.assign(tmp_c, size_c);
    }
    return	buf;
}

#if	!defined(__GNUC__) || defined (_GLIBCXX_USE_WCHAR_T) || defined (_GLIBCXX_USE_WSTRING)
buffer& operator<<(buffer& buf, const std::wstring& value) {
    WriteSerialString(buf, value.c_str(), MinValue(value.size(), GC_SERIALIZE_STRING_MAX_SIZE));

    return	buf;
}

buffer& operator>>(buffer& buf, std::wstring& value) {
    wchar_t			tmp_v[GC_SERIALIZE_STRING_MAX_SIZE];
    unsigned long	size	= ReadSerialString(buf, tmp_v, sizeof(tmp_v)/sizeof(tmp_v[0]));
    if(buf.good()) {
        value.assign(tmp_v, size);
    }
    return	buf;
}
#endif
#else
static	bool	WriteSerialString(buffer& buf, const char* data, unsigned long size) {
    buffer_tag	tag	= {
        buffer_tag::TYPE_STRING,
        buffer_size_tag(size),
        false
    };

    return	(	buffer_write_tag(buf, tag)
                &&		buffer_write_uint_value(buf, tag.size_tag, size)
                &&		buf.write(data, size)
           );
}

static	unsigned long	ReadSerialString(buffer& buf, char* data, unsigned long size) {
    unsigned long	size_c;
    buffer_tag		tag;
    if(		!buffer_read_tag(buf, tag)
            ||	!buffer_read_uint_value(buf, int(tag.size_tag), size_c)
            ||	size_c > GC_SERIALIZE_STRING_MAX_SIZE*3
            ||	size_c > size
            ||	!buf.read(data, size_c)
      ) {
        buf.set_failure();
        return	0;
    }

    return	size_c;
}

buffer& operator<<(buffer& buf, const std::string& value) {
    WriteSerialString(buf, value.c_str(), MinValue(value.size(), GC_SERIALIZE_STRING_MAX_SIZE*3));

    return	buf;
}
buffer& operator>>(buffer& buf, std::string& value) {
    char			tmp_v[GC_SERIALIZE_STRING_MAX_SIZE*3];
    unsigned long	size	= ReadSerialString(buf, tmp_v, sizeof(tmp_v)/sizeof(tmp_v[0]));
    if(buf.good()) {
        value.assign(tmp_v, size);
    }
    return	buf;
}

#if	!defined(__GNUC__) || defined (_GLIBCXX_USE_WCHAR_T) || defined (_GLIBCXX_USE_WSTRING)
buffer& operator<<(buffer& buf, const std::wstring& value) {
    char			tmp_v[GC_SERIALIZE_STRING_MAX_SIZE*3 + 1];
    unsigned long	tmp_c	= string_wchar_to_utf8(value.c_str(), value.size()*sizeof(wchar_t),
                              tmp_v, sizeof(tmp_v));
    WriteSerialString(buf, tmp_v, tmp_c);

    return	buf;
}

buffer& operator>>(buffer& buf, std::wstring& value) {
    char			tmp_v[GC_SERIALIZE_STRING_MAX_SIZE*3 + 1];
    wchar_t			tmp_w[GC_SERIALIZE_STRING_MAX_SIZE];
    unsigned long	size_c	= ReadSerialString(buf, tmp_v, sizeof(tmp_v));
    tmp_v[size_c]	= 0;

    unsigned long	size_w	= string_utf8_to_wchar(tmp_v, size_c,
                              tmp_w, sizeof(tmp_w));

    if(buf.good()) {
        value.assign(tmp_w, size_w/sizeof(wchar_t));
    }

    return	buf;
}
#endif
#endif

buffer& operator<<(buffer& buf, const buffer& value) {
    buffer_write_raw(buf, (void*)value.data(), value.size());
    return buf;
}

buffer& operator>>(buffer& buf, buffer& value) {
    size_t nLen = value.capacity();
    std::auto_ptr<uint8_t>	pData(new uint8_t[nLen]);
    if (!buffer_read_raw(buf, pData.get(), nLen)) {
        value.set_failure();
    } else {
        value.write(pData.get(), nLen);
    }

    return buf;
}
