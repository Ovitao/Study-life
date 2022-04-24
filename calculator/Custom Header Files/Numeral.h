/*

4/23/2022 negative numeral conversions added
tested from -2000 to 2000
*/
#include "std_lib_facilities.h"
#include "token.h"

//implements conversions of numeral systems

//Numeral_system{
//public:
namespace  Numeral_system 
{
	int octal_to_decimal(string);
	int binary_to_decimal(string);
	int hexa_to_decimal(string);
	string decimal_to_binary(int);
	string decimal_to_hexa(int);
	string decimal_to_octal(int);
	string get_numeral_from_decimal(int decimal, Token_stream& key);
	int get_decimal_from_numeral(const char& key, Token_stream& numeral);
	//}
	char oct_from_bin(const string& three_bit); // three bit binary to octal
	string read_numeral(Token_stream&);
	int what_is(const char&); //hex-list
	char what_is(const int&); //hex-list
	char hex_from_bin(const string& quartet); // four bit binary to hex
}