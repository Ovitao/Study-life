



#include "Costom Header Files/std_lib_facilities.h"
#include "Costom Header Files/Other.h"





char lc(char c)
{
	const int lcase = 32;
	if (c >= 'A' && 'Z' >= c) c += lcase;
	if (c >= 'À' && 'ß' >= c) c += lcase;
	return c;
}
bool is_alpha(char c)
{
	return (lc(c) >= 'a' && 'z' >= lc(c))  //ENG
		|| (lc(c) >= 'à' && 'ÿ' >= lc(c)); //RUS
}
bool is_digit(char c)
{
	return c >= '0' && '9' >= c; 
}
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

		if (c == '.' && what == floating) error("Bad format");
		if (c < '0' || '9' < c) what = 'd';

		switch (what)
		{
		case integer:
			d *= base;
			d += (c - '0');
			break;
		case floating: //once we get here we won't go into integer:
			d += (c - '0') / base;
			base *= 10;
			break;
		default:

			if (c != floating && (c < '0' || '9' < c)) error("Bad format");
			what = floating;
		}
	}
	return d *= sign;
}