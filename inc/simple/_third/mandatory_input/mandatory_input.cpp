/**
** @file		mandatory_input.cpp
**
** @brief		implementation of class mandatory_input
**
** @detail		Format controller for istream extraction with enhanced error processing
**
** @remark		The class developped following an idea on Stack Overflow:  http://stackoverflow.com/a/14331519/3723423
**				The original idea was considerably enhanced to facilitate flexible error processing.  See also:
**				http://stackoverflow.com/questions/24726307/improving-error-processing-for-an-istream-helper-class-when-using-exceptions
**
** @author		Christophe T.
** @copyright	ZLIB License / see license.txt
**/

#include <cctype>
#include <istream>
#include "mandatory_input.h"

char mandatory_input::last = 0;
char mandatory_input::expected = 0;
char mandatory_input::read_error = 0;

mandatory_input::mandatory_input(char const *fmtstr)
	: str(fmtstr)
{}

char mandatory_input::reset()
{
	expected = read_error = 0;
	char c = last;
	last = 0;
	return c;
}

//mandatory_input::failure::failure(std::error_code ec)
//	: std::istream::failure("Mandatory input character not found",ec)
//{
//}

std::istream &operator>> (std::istream &is, mandatory_input const &fmt) 
{
	int c;
	if (is)	{	//  avoid overwriting of error messages in case of previous problems 
		for (char const *p = fmt.str; *p; ++p) {
			if (std::isspace(*p)) {
				std::istream::sentry ws(is); // discard whitespace
			}
			else if ((c = is.get()) != *p) {   //===== input char not matching expected char
				is.putback(c);  // the failed char shall be the next onte read. 
				mandatory_input::read_error = c;  // document error conditions
				mandatory_input::expected = *p;
				if (is.exceptions() & std::istream::failbit) { // if streams foresees exceptions 
					try {                                  // set the failbit, but catch failure 
						is.setstate(std::ios::failbit);
					}
					catch (std::istream::failure &/*e*/) {
						//throw mandatory_input::failure(e.code()); // and rethrow specialized exception
						throw std::istream::failure("Mandatory input character not found"); // and rethrow specialized exception
					}
					catch (std::exception &e) {
						throw e;
					}
				}
				else						// if streams foresses no exceptions, siply set the failbit
					is.setstate(std::ios::failbit); 
			}
			else {                   // if char matched, document progress 
				mandatory_input::last = c;
				mandatory_input::expected = 0;
			}
		}
	}
	return is;
}
