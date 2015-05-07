/**
** @file		test.cpp
**
** @brief		test and example of use of mandatory_input class
**
** @detail		test with and witout exceptions to read a list of pairs.  It runs each test on a correct and on an incorrect string stream
**              and gives opportunity to manually provide additional test input
**
** @author		Christophe T.
** @copyright	ZLIB License / see licence.txt
**/

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cctype>
#include <cstdio>
#include <regex>
#include "mandatory_input.h"

using namespace std;
using namespace std::chrono;

///  test function to read an input stream for a list of pairs spearated by commas. 
/**
**   @remark	If a comma doesn't follow a pair, it means we are at the end of the list.  
**              Other formatting errors are not tolerated and need explanations.  
*/
bool test_wthout_exceptions(istream& is)
{
	bool ret = true; 
	int x = 0, y = 0;

	// Reading loop trying to read (x,y),  
	while ((is >> mandatory_input(" ( ") >> x >> mandatory_input(" , ") >> y >> mandatory_input(" ) , ") ))
	{
		cout << "[" << x << ";" << y << "]" << mandatory_input::getlast();
	}
	// the loop could end on any kind of error, and eof.  
	if (is.fail())		// Error processing in case of invalid input
	{
		if (mandatory_input::getlast() == ')')  // if only the comma separator between pairs is missing
			cout << "[" << x << ";" << y << "]" << "End of list" << endl;  // use the successful input 
		else if (mandatory_input::error()) {    // but if a more serious error ended the loop, explain the issue
			cout << x << " " << y << " " << " last succesfully controlled char:" << mandatory_input::getlast() << endl;
			cout << mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";
			ret = false; 
		}
		is.clear();  // in any case reset failbit to continue reading
		string s;
		getline(is, s);
		cout << "Remaining of input line:" << s << endl;   // if format error, the first char should be the one that lead to the error
	}
	return ret; 
}

///  test function to read an input stream for a list of pairs spearated by commas but with exception handling on errors. 
/**
**   @remark	see test_wthout_exceptions() for details.  
*/

bool test_with_exceptions(istream& is)
{
	bool ret = true;
	int x = 0, y = 0;

	// set excpetion handling on
	is.exceptions(std::istream::failbit | std::istream::badbit);

	// Reading loop
	try {
		while ((is >> mandatory_input(" ( ") >> x >> mandatory_input(" , ") >> y >> mandatory_input(" ) , ")))
		{
			cout << "[" << x << ";" << y << "]" << mandatory_input::getlast() ;
		}
	}
	catch (mandatory_input::failure e) {
		if (mandatory_input::getlast() == ')')
			cout << "[" << x << ";" << y << "]" << "End of list" << endl;
		else {
			cout << is.fail() << " MANDATORY failure: " << e.code() << " " << e.what() << endl;
			cout << x << " " << y << " " << " last succesfully controlled char:" << mandatory_input::getlast() << endl;
			cout << mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";
			ret = false;
		}
		is.clear();  // reset failbit to continue reading
		string s;
		getline(is, s);
		cout << "Remaining of input line:" << s << endl;   // if format error, the first char should be the one that lead to the error
	}
	catch (istream::failure e) {   // THIS SHOULDN'T HAPPEN for formatting errors 
		cout << is.fail() << " Input failure: " << e.code() << " " << e.what() << endl;
		cout << x << " " << y <<  " last succesfully controlled char:" << mandatory_input::getlast() << endl;
		if (mandatory_input::error()) // if we had a formatting error 
			cout << mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";
		is.clear();  // reset failbit to continue reading
		string s;
		//getline(is, s);
		cout << "Remaining of input line:" << s << endl;   // if format error, the first char should be the one that lead to the error
	}
	return ret;
}


///  benchmark function to read an input stream with isbn 
/**
**   @remark	compares stream extraction with or without mandatory_input, 
**              and compares with scanf(), uparsed input, and regex 
*/

void benchmark()
{
	volatile long long n[10]{};
	int lg, rg, bk, chk;
	char c; 
	string s; 
	vector<high_resolution_clock::time_point> tstart, tend; 
	vector<string>scenario; 
	regex rx("([0-9])-([0-9][0-9][0-9])-([0-9])*-([0-9])");
	smatch sm; 

	cout << "BENCHMARKING:" << endl; 
	ifstream ifs("test.txt");

	scenario.push_back("getline+regex   "); 
	tstart.push_back(high_resolution_clock::now());
	for (int i = 0; i < 1000; i++) {
		while (getline(ifs, s)) {
			regex_match(s, sm, rx);
			lg = stoi(sm[1]);
			rg = stoi(sm[2]);
			bk = stoi(sm[3]);
			chk = stoi(sm[4]);
			n[0]++;
		}
		ifs.clear(); ifs.seekg(0, ios::beg);
	}
	tend.push_back(high_resolution_clock::now());

	scenario.push_back("stream extract 1");
	tstart.push_back(high_resolution_clock::now());
	for (int i = 0; i < 1000; i++) {
		while (getline(ifs, s))
			n[1]++;
		ifs.clear(); ifs.seekg(0, ios::beg);
	}
	tend.push_back(high_resolution_clock::now());

	scenario.push_back("stream extract 4");
	tstart.push_back(high_resolution_clock::now());
	for (int i = 0; i < 1000; i++) {
		while (ifs >> lg >> c  >> rg >> c >> bk >> c>> chk)
			n[2]++;
		ifs.clear(); ifs.seekg(0, ios::beg);
	}
	tend.push_back(high_resolution_clock::now());

	scenario.push_back("stream ext.mandat");
	tstart.push_back(high_resolution_clock::now());
	for (int i = 0; i < 1000; i++) {
		while (ifs >> lg >> mandatory_input("-") >> rg >> mandatory_input("-") >> bk >> mandatory_input("-") >> chk)
			n[3]++;
		ifs.clear(); ifs.seekg(0, ios::beg);
	}
	tend.push_back(high_resolution_clock::now());
	ifs.close();

	FILE*fp= fopen("test.txt", "r");
	scenario.push_back("C file scanf     ");
	tstart.push_back(high_resolution_clock::now());
	for (int i = 0; i < 1000; i++) {
		while (fscanf(fp,"%d-%d-%d-%d", &lg,&rg,&bk,&chk)==4) 
			n[4]++;
		fseek(fp, 0, 0); 
	}
	tend.push_back(high_resolution_clock::now());
	fclose(fp);

	for (int i = 0; i < scenario.size(); i++) {
		long dt = duration_cast<milliseconds>(tend[i] - tstart[i]).count();
		cout << scenario[i]<<"\t" << dt << " ms for " << n[i] << " elements" << endl;
	}

}

///  launches the tests

int main(int ac, char**av)
{
	cout << "Test for formatted input\n";
	string retry;

	int lg, rg, bk, chk; 
	istringstream isbn1("0-201-70073-5"), isbn2("0-201/70073-5"), isbn3("0-20170073-5");

	if (isbn1 >> lg >> mandatory_input("-") >> rg >> mandatory_input("-") >> bk >> mandatory_input("-") >> chk)
		cout << "ISBN1 ok:" << lg << "-" << rg << "-" << bk << "-" << chk << endl;
	else cout << "ISBN1 failed: "<< mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";

	if (isbn2 >> lg >> mandatory_input("-") >> rg >> mandatory_input("-") >> bk >> mandatory_input("-") >> chk)
		cout << "ISBN2 ok:" << lg << "-" << rg << "-" << bk << "-" << chk << endl;
	else cout << "ISBN2 failed: " << mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";

	if (isbn3 >> lg >> mandatory_input("-") >> rg >> mandatory_input("-") >> bk >> mandatory_input("-") >> chk)
		cout << "ISBN3 ok:" << lg << "-" << rg << "-" << bk << "-" << chk << endl;
	else cout << "ISBN3 failed: " << mandatory_input::getexpected() << " was expected, but " << mandatory_input::getread_error() << " was read !\n";

	istringstream insok("(1,2),(2,\t4) , ( 4 , 6 ),(5, 9) finished!"),
		insnok("(1,2),(2+4) , ( 4 , 6 ),(5, 9) finished!");

	cout << "=======================================================\n";
	cout << "Test controlled input format (without exceptions)\n\n";
	cout << "Input is a list of comma separated pairs (x,y).\nMissing comma between pairs means end of list: \n";
	cout << "\nFirst test: " << insok.str() << endl; 
	if (test_wthout_exceptions(insok))
		cout << "Good string stream parsed successfully: TEST OK\n";
	else cout << "Good string stream parsed unsuccessfully: TEST FAILED\n";
	cout << "\nSecond test: " << insnok.str() << endl;
	if (test_wthout_exceptions(insnok)==false)
		cout << "Bad string stream parsed unsuccessfully: TEST OK\n";
	else cout << "Bad string stream parsed successfully: TEST FAILED\n";
	cout << "\nThird test: enter manually pairs.  The first error will end input :"  << endl;
	do {
		test_wthout_exceptions(cin);
		cout << "Do you want to retry (Y/N) ? ";
		getline(cin, retry);
	} while (toupper(retry[0]) == 'Y');

	insok.seekg(0, ios::beg);  // rewind the string streams
	insnok.seekg(0, ios::beg);
	cout << "=====================================================\n";
	cout << "Test controlled input format (with exceptions)\n\n";
	cout << "Input is a list of comma separated pairs (x,y).\nMissing comma between pairs means end of list: \n";
	cout << "\nFirst test: " << insok.str() << endl;
	if (test_with_exceptions(insok))
		cout << "Good string stream parsed successfully: TEST OK\n";
	else cout << "Good string stream parsed unsuccessfully: TEST FAILED\n";
	cout << "\nSecond test: " << insnok.str() << endl;
	if (test_with_exceptions(insnok) == false)
		cout << "Bad string stream parsed unsuccessfully: TEST OK\n";
	else cout << "Bad string stream parsed successfully: TEST FAILED\n";
	cout << "\nThird test: enter manually pairs.  The first error will end input :" << endl;
	do {
		test_with_exceptions(cin);
		cout << "Do you want to retry (Y/N) ? ";
		getline(cin, retry);
	} while (toupper(retry[0]) == 'Y');

	benchmark();

	cout << "Press any key...";
	cin.get(); 
	return 0; 
}
