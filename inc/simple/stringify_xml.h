#ifndef STRINGIFY_XML_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRINGIFY_XML_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<iosfwd>

struct	stringify_data;

bool	stringify_to_xml_stream(stringify_data& data, std::ostream& os, const std::string& anonymous_tag_name = "_");
bool	stringify_from_xml_stream(stringify_data& data, std::istream& is, const std::string& anonymous_tag_name = "_");

#endif
