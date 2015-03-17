#ifndef STREAM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STREAM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <iosfwd>
#include <string>

//	bool
bool	stream_dump(std::ostream& os, int level, bool value);

//	number
bool	stream_dump(std::ostream& os, int level, signed char value);
bool	stream_dump(std::ostream& os, int level, unsigned char value);
bool	stream_dump(std::ostream& os, int level, signed short value);
bool	stream_dump(std::ostream& os, int level, unsigned short value);
bool	stream_dump(std::ostream& os, int level, signed long value);
bool	stream_dump(std::ostream& os, int level, unsigned long value);
bool	stream_dump(std::ostream& os, int level, signed int value);
bool	stream_dump(std::ostream& os, int level, unsigned int value);

bool	stream_dump(std::ostream& os, int level, float value);
bool	stream_dump(std::ostream& os, int level, double value);

//	string
bool	stream_dump(std::ostream& os, int level, const std::string& value);
//bool	stream_dump(std::ostream& os, int level, const std::wstring& value);

//	object
bool	stream_dump_object_begin(std::ostream& os, int level, const char* type_name);
bool	stream_dump_object_end(std::ostream& os, int level, const char* type_name);
bool	stream_dump_object_field_begin(std::ostream& os, int level, const char* field_name);
bool	stream_dump_object_field_end(std::ostream& os, int level, const char* field_name);

template<typename FieldType>
bool	stream_dump_object_field(std::ostream& os, int level, const char* field_name, const FieldType& value) {
    stream_dump_object_field_begin(os, level, field_name);
    stream_dump(os, level + 1, value);
    stream_dump_object_field_end(os, level, field_name);

    return	os.good();
}

template<typename ObjectType>
bool	stream_dump_object(std::ostream& os, const ObjectType& obj) {
    return	stream_dump(os, 0, obj);
}

//	stl
bool	stream_dump_array_begin(std::ostream& os, int level);
bool	stream_dump_array_end(std::ostream& os, int level);
bool	stream_dump_array_item_begin(std::ostream& os, int level, int index);
bool	stream_dump_array_item_end(std::ostream& os, int level, int index);
template<typename ArrayType>
bool	stream_dump(std::ostream& os, int level, const ArrayType& array) {
    stream_dump_array_begin(os, level);

    size_t	size	= array.size();
    typename ArrayType::const_iterator it	= array.begin();
    for(size_t i = 0; i < size; ++i, ++it) {
        stream_dump_array_item_begin(os, level, int(i + 1));
        stream_dump(os, level + 1, *it);
        stream_dump_array_item_end(os, level, int(i + 1));
    }

    stream_dump_array_end(os, level);

    return	os.good();
}
// pair helper.
template<class KEY, class VALUE>
bool	stream_dump(std::ostream& os, int level, const std::pair<KEY,VALUE>& value) {
    stream_dump_array_begin(os, level);

    stream_dump_array_item_begin(os, level, 1);
    stream_dump(os, level + 1, value.first);
    stream_dump_array_item_end(os, level, 1);

    stream_dump_array_item_begin(os, level, 2);
    stream_dump(os, level + 1, value.second);
    stream_dump_array_item_end(os, level, 2);

    stream_dump_array_end(os, level);

    return	os.good();
}

//	raw
class	buffer;
bool	stream_dump(std::ostream& os, int level, const buffer& value);

#endif
