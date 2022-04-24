



#include "Custom Header Files/std_lib_facilities.h"
#include "Custom Header Files/Other.h"

//----------------------------
// to avoid a fatal error caused by some functions such as isdigit,isalpha,isspase etc. when Cyrillic letter is used
bool is_latin(char r) 
{
	if (lc(r) >= 'a' && 'z' >= lc(r)) return true;  
	return false;
}
//----------------------------
string revers_string(string s) 
{
	if (s.size() == 1 || s.size() == 0) return s;
	char hold;
	for (int i = 0; i <= (s.size() - i - 1); ++i) {
		hold = s[i];
		s[i] = s[s.size() - i - 1];
		s[s.size() - i - 1] = hold;
	}
	return s;
}
//----------------------------
bool is_space(char ch)
{
	return (ch == ' ' || ch == '\n' || ch == '\t');
}
//----------------------------
//  factorial
double ft(double num) 
{
	if (num == 0 || num == 1) return 1;
	if (num < 0) error("factorial with negative number");
	if (num - int(num) != 0)  error("factorial: only integers are allowed");

	for (double mult = num - 1; mult > 1; mult -= 1)
		num *= mult;
	return num;
}
//----------------------------
// get lower case letter
char lc(char c) 
{
	if ('A' <= c && c <= 'Z')return c += 32;
//	if ('À' <= c && c <= 'Ï'|| 'Ð' <= c && c <= 'ß' ) return c += 48;
	return c;
}
//----------------------------
// get upper case letter
char uc(char c)
{
	if ('a' <= c && c <= 'z') return c -= 32;
//	if ('à' <= c && c <= 'ï' || 'ð' <= c && c <= 'ÿ') return c -= 48;
	return c;
}
//----------------------------
// is letter
bool is_letter(const char& c)
{
	return ('a' <= lc(c) && lc(c) <= 'z');  //ENG
	//	|| ('à' <= lc(c) && lc(c) <= 'ï' || 'ð' <= lc(c)&& lc(c) <= 'ÿ'); //RUS
}
//----------------------------
bool is_digit(const char& c)
{
	return c >= '0' && '9' >= c; 
}
//----------------------------
double string_to_double(string s)
{
	int sign{ 1 };
	if (s[0] == '-') {
		sign = -1; s[0] = '0';
	}
	else if (s[0] == '+')  s[0] = '0';

	const char integer{ '0' };
	const char floating{ '.' };
	char what = integer;
	double d{ 0 };
	double base{ 10.0 };

	for (const char& c : s) {

		if (c == '.' && what == floating) error("string_to_double: Bad format");
		if (c < '0' || '9' < c) what = 'd';

		switch (what)
		{
		case integer:
			d *= base;
			d += (c - '0');
			break;
		case floating: //once we get here, we won't go into the integer
			d += (c - '0') / base;
			base *= 10;
			break;
		default:

			if (c != floating && (c < '0' || '9' < c)) error("string_to_double: Bad format");
			what = floating;
		}
	}
	return d *= sign;
}
//----------------------------