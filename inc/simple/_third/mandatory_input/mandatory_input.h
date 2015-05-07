/**
 ** @file		mandatory_input.h 
 **
 ** @brief		header declaring class mandatory_input
 **
 ** @detail		Format controller for istream extraction with enhanced error processing
 **
 ** @remark		The class developped following an idea on Stack Overflow:  http://stackoverflow.com/a/14331519/3723423
 **				The original idea was considerably enhanced to facilitate flexible error processing.  See also:
 **				http://stackoverflow.com/questions/24726307/improving-error-processing-for-an-istream-helper-class-when-using-exceptions		
 **
 ** @author		Christophe T.
 ** @copyright	ZLIB License / see licence.txt
**/

#ifndef MANDATORY_INPUT_H
#define MANDATORY_INPUT_H

#include <istream>

/** @brief		Format controler for istream extraction 
  *
  * @detail		Checks for a sequence of one or several expected formatting characters that shall be skipped from the
  *				stream. Space formatting characters are understood as optional whitespace that shall be skipped.
  *
  *				In case of non compliant input:
  *				    * the failbit is set, like for any invalid input in istream opreator>>
  *					* If exceptions are set for the istream, specific formatting exceptions are then raised.
  *					* The next char to be read in the stream  will the char that didn't match expectatins.
  *
  * @headerfile	""
  * @attention	class designed to be used as temporary, it doesn't keep a copy of the elements provided at construction. 
  */

class mandatory_input
{
public:

	/// Constructor used to initialize from a string litteral
	/**  
	  *  @param fmstr Sequence of one or several format characters that must appear in the given order. 
	  *				 Each space is understood as accepting optional whitespaces. 
	  *  @attention	 No copy is made of the c string.  The original point must be valid when object is used. 
	  */
	mandatory_input(char const *fmtstr);

	/// Stream extractor 
	/** 
	 *  Extracts characters from stream and verifies their compliance with expected format characters
	 *  @fn			 std::istream &operator>> (std::istream &is, mandatory_input const &fmt);
	 *  @relates	 mandatory_input
	 *  @param is	 stream for input
	 *  @param fmt	 mandatory_input object the input must comply with
	 *  @return 	 stream used for input
	 *  @remark		 In case of non compliant input:
	 *				    * the failbit is set, like for any invalid input in istream opreator>>
	 *					* If exceptions are set for the istream, specific formatting exceptions are then raised.
	 *					* The next char to be read in the stream  will the char that didn't match expectatins.
 	 *  @example	 operator>> std::cin >> mandatory_input(" [ ") >> i >> mandatory_input (" ] , ");
	 */
	friend std::istream &operator>> (std::istream &is, mandatory_input const &fmt);  

	static inline char getlast() 		/// Gets last non white successfully controlled char
		{ return last; }
	static inline char getexpected()	/// Gets the expected format char that was missing and lead to a failure. 
		{ return expected; }			// returns 0 if no error was encountered !
	static inline char getread_error()	/// Gets the read char that was non compliant and lead to the last error.  
		{ return read_error; }
	static char reset();				///< Gets last non white successfully controlled char and resets it to 0
	static inline bool error()			/// Tells if there was an error since the last reset or the last successful reading
		{ return expected != 0; }	         
	/// Exception that specializes istream::failure in case of non compliant input 
	/**
	 *  Exception is raised when input is non compliant, but only if input stream has exception enabled on the failbit
	 *  @remark		The caller is free to catch specifically for mandatory_input::failure or not to bother with details 
	 *				and catch the standard istream::failure
	**/
//	class failure : public std::istream::failure   
//	{ 
//	/// Constructor with error code
//	public: failure(std::error_code e); 
//	};
private:
	char const *str; 					///< Formatting chars provided by the constructor  
	static char last,					///< Last non white successfully controlled char
		expected,						///< Last formatting char that could not be found 
		read_error;						///< Last read character that wasn't copmliant
};



#endif