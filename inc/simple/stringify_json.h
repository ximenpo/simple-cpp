#ifndef STRINGIFY_JSON_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRINGIFY_JSON_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<iosfwd>

struct	stringify_data;

bool	stringify_to_json_stream(stringify_data& data, std::ostream& os);
bool	stringify_from_json_stream(stringify_data& data, std::istream& is);

#endif
