#include <simple/igloo.h>
using namespace igloo;

#include <fstream>
#include <simple/app.h>
#include <simple/csv.h>
#include <simple/string.h>

Context(csv_usage) {
    Spec(basic_usage) {
        std::ifstream infile("tests/test_data/sample.csv", std::ios::binary);
        AssertThat(bool(infile),	IsTrue());

        csv_line parser;
        std::ostringstream	os;

        while (infile) {
            std::string sLine;
            std::getline(infile, sLine, char('\r')); // Get a line
            if (sLine == "")
                continue;

            parser << sLine; // Feed the line to the parser

            // Now extract the columns from the line
            std::string sCol1, sCol3, sCol4;
            double fCol2;
            int iCol5, iCol6;
            parser >> sCol1 >> fCol2 >> sCol3;
            parser >> sCol4 >> iCol5;
            parser >> iCol6;

            os << "Column1: " << sCol1 << std::endl
               << "Column2: " << fCol2 << std::endl
               << "Column3: " << sCol3 << std::endl
               << "Column4: " << sCol4 << std::endl
               << "Column5: " << iCol5 << std::endl
               << "Column6: " << iCol6 << std::endl
               << std::endl;

            if(infile.peek() == '\n') {
                infile.get();
            }
        }
        infile.close();

        AssertThat(os.str(),	Equals(
                       "Column1: Word1\n"
                       "Column2: 0.51\n"
                       "Column3: Word2 Word3 Word4\n"
                       "Column4: Potzrebie\n"
                       "Column5: 22\n"
                       "Column6: 44\n"
                       "\n"
                       "Column1: More words\n"
                       "Column2: 0.62\n"
                       "Column3: The grand panjandrum\n"
                       "Column4: Wumplekins was here\n"
                       "Column5: 33\n"
                       "Column6: 63\n"
                       "\n"
                   ));
    }

    Spec(excel_csv_file_usage) {
        std::ifstream infile("tests/test_data/excel.csv", std::ios::binary);
        AssertThat(bool(infile),	IsTrue());

        csv_line parser;
        std::ostringstream	os;

        while (infile) {
            infile >> parser;
            if(parser.empty()) {
                break;
            }

            // Now extract the columns from the line
            std::string sCol1, sCol2, sCol3;
            parser >> sCol1 >> sCol2 >> sCol3;

            os << "Column1: " << sCol1 << std::endl
               << "Column2: " << sCol2 << std::endl
               << "Column3: " << sCol3 << std::endl
               << std::endl;
        }
        infile.close();

        AssertThat(string_replace(os.str().c_str(), "\r", ""),	Equals(
                       "Column1: a\n"
                       "Column2: b\n"
                       "Column3: c\n"
                       "\n"
                       "Column1: 1\n"
                       "Column2: 1\n2\n"
                       "Column3: c \"c\n"
                       "\n"
                       "Column1: 1\n"
                       "Column2: 1\n2\n"
                       "Column3: c c\n"
                       "\n"
                   ));
    }
};
