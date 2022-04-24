/* tested up to 2500
	4.19.2022
*/
#include "std_lib_facilities.h"

// full conversion to roman numeral
string int_to_roman(const int& number); // output in uppercase numerals

// converts some numbers
string what_is(const int&); 

// converts some numerals
int what_is(const string&); 

// list of some numerals
bool is_numeral(const string&); 

bool is_format(const string& single_numeral, const string& single_numeral_next);

// full conversion to int
int roman_to_int(string numeral); // also makes 'numeral' lower case for other functions




/*
class Roman
{
	string numeral;
	int number;
	Roman(string l, int n)
		:numeral{ l }, number{ n } {}
};
*/

