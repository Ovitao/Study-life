/* tested up to 2500
	4.19.2022
*/
#include "Custom Header Files/Other.h"
#include "Custom Header Files/roman_int.h"

//------------------------------------------------------------------------------------
int what_is(const string& numeral)
// numeral checks before call of a function in roman_to_int()
{
	int num{ 0 };

	for (const char& c : numeral)num += int(c);

	switch (num)
	{
		case 'I' : return 1;
		case 'I' + 'I': return 2;
		case 'I' + 'I' + 'I': return 3;
		case 'I' + 'V' : return 4;
		case 'V' : return 5;
		case 'I' + 'X' : return 9;
		case 'X' : return 10;
		case 'X' + 'X' : return 20;
		case 'X' + 'X' + 'X': return 30;
		case 'X' + 'L' : return 40;
		case 'L' : return 50;
		case 'X' + 'C' : return 90;
		case 'C' : return 100;
		case 'C' + 'C': return 200;
		case 'C' + 'C' + 'C': return 300;
		case 'C' + 'D' : return 400;
		case 'D' : return 500;
		case 'C' + 'M' : return 900;
		case 'M' : return 1000;
		case 'M' + 'M' : return 2000;
		case 'M' + 'M' + 'M': return 3000;
		default:
			error("bad roman numeral example");
	}
}
//------------------------------------------------------------------------------------
bool is_format(const string& single_numeral, const string& single_numeral_next)
// Checks following types of errors:
// more than 3 'I' or '' or 'C' or 'M' in a row or more than two the same 2-char-single_numeral e.g. IIII or  IVIV
// single_numeral_1 and single_numeral_next can be represented as a single_numeral e.g.DD == M
// single_numeral < single_numeral_next  e.g. IIX or IC
// or single_numeral is not like in the is_numeral() function list
{
	if (!is_numeral(single_numeral) || !is_numeral(single_numeral_next)) return false; // if not in the list
	if (what_is(single_numeral) < what_is(single_numeral_next)) return false; // e.g. iix or ic
	if (single_numeral.size() == 3 && single_numeral[2] == single_numeral_next[0]) return false; // e.g. iiii
	if (single_numeral.size() == 2 && single_numeral == single_numeral_next) return false; // e.g.iviv
	if (is_numeral(int_to_roman(what_is(single_numeral) + what_is(single_numeral_next)))) return false; //e.g. dd == m 

	return true;
}
//------------------------------------------------------------------------------------
bool is_numeral(const string& s)
// we need so many for the fromat check in the is_format() function.
{
	vector<string> x{ "I", "II","III", "IV", "V", "IX", "X", "XX","XXX", "XL",
						"L", "XC", "C", "CC", "CCC", "CD", "D", "CM", "M","MM", "MMM" };
	for (const string& c : x) if (c == s) return true; // search
	return false;
}
//------------------------------------------------------------------------------------
int roman_to_int(string numeral)
{
	for (char& c : numeral) c = uc(c); // make uppercase numeral
	string single_numeral_0; // to check format later
	string single_numeral_1;
	int number{};

	numeral.push_back(char(-1)); // terminator for the search below

	for (const char& n : numeral)
		if (is_numeral(single_numeral_1 + n)) single_numeral_1.push_back(n);
		else {
			number += what_is(single_numeral_1); // convert the current single_numeral increment
			if (!single_numeral_0.empty()) // compare nearest single numerals
				if (!is_format(single_numeral_0, single_numeral_1)) error("bad roman numeral format - ", single_numeral_0 + single_numeral_1);
			single_numeral_0 = single_numeral_1; // save the current for comparison
			single_numeral_1 = string{}; // start a new
			single_numeral_1.push_back(n);
		}
	return number;
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
string what_is(const int& n)
{
	switch (n)
	{
	case 1: return "I";
	case 4: return "IV";
	case 5: return "V";
	case 9: return "IX";
	case 10: return "X";
	case 40: return "XL";
	case 50: return "L";
	case 90: return "XC";
	case 100: return "C";
	case 400: return "CD";
	case 500: return "D";
	case 900: return "CM";
	case 1000: return "M";
	}
	error("bad example of template for a Roman numeral", n);
}
//------------------------------------------------------------------------------------
string int_to_roman(const int& number)
{ 
	if (number < 1) { error("raman numeral can't be 0 or negative"); }
	int reference{};
	string rom_numeral{};
	vector<int> reflist{ 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000, -1 }; // if we got to -1 it means 1000 is our nearest increment
	
	// make closest increment to the number in each turn
	while (reference != number)
		for (int i{ 0 }; i < reflist.size(); ++i)
		{     
			if (reference + reflist[i] > number || reflist[i] == -1) // -1 is a terminator, it means end of the loop and no result
			{
				rom_numeral += what_is(reflist[i - 1]);
				reference += int(reflist[i - 1]);
				break; // look up the next increment or end
			}
		}
	return rom_numeral;
}
//------------------------------------------------------------------------------------
