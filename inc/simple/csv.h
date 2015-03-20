#ifndef CSV_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CSV_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<iosfwd>
#include	"third/csvparser.h"

class	csv_line	: public CSVParser {
public:
    void	clear();
    bool	empty();
    friend	std::istream& operator >>(std::istream& is, csv_line& line);
};

#endif
