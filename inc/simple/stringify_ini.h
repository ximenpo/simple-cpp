#ifndef STRINGIFY_INI_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRINGIFY_INI_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<iosfwd>

struct	stringify_data;

//
//	!	Note:	from func was not support non-leaf array.
//				��ȡ������֧�� �м�����ڵ㣬�� ��������飬��������顣
//
bool	stringify_to_ini_stream(stringify_data& data, std::ostream& os, const std::string& path_delimeter = "/");
bool	stringify_from_ini_stream(stringify_data& data, std::istream& is, const std::string& path_delimeter = "/");

#endif
