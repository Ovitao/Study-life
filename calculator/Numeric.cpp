




#include "Custom Header Files/Numeric.h"
#include "Custom Header Files/token.h"
#include "constants.cpp"
#include "Custom Header Files/Other.h"


//-------------------------------------------------------------------------------------------------------------------------------
string Numerical_sysem::get_literal_from_decimal(double d) 
{
//works with rational positive numbers
//the double for the big numbers
	Token_stream ts(cin);

	Token t = ts.get();
	switch (t.kind)
	{
	case hexadecimal:
		return decimal_to_hexa(d);
	case octal:
		return decimal_to_octal(d);
	case binary:
		return decimal_to_binary(d);
	default:
		error("Numerical system key expected.\n");
		break;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------

double Numerical_sysem::get_decimal_from_literal(char c) 
{
	//works with rational positive numbers
	//the double for the big numbers
	char nl;
	string literal_number;
	while (cin.get(nl) && islatin(nl) && isspace(nl) && nl != print);

	if (!(islatin(nl) && isalpha(nl) || isdigit(nl))) { cin.unget(); error("literal expected."); }

	literal_number += nl;
	while (cin.get(nl) && islatin(nl) && isalpha(nl) || isdigit(nl)) literal_number += nl;

	cin.unget();
	switch (c)
	{
	case binary:
		return binary_to_decimal(literal_number);
	case hexadecimal:
		return hexa_to_decimal(literal_number);
	case octal:
		return octal_to_decimal(literal_number);
	default:
		error("get: Numerical_system.\n");
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------

double Numerical_sysem::octal_to_decimal(string octal) 
{
	//works with rational positive numbers
	//the double is for the big numbers
	if (octal.size() == 1 && octal == "0") return 0;
	double d;
	double result = 0;
	int power = 0;
	const int base = 8;
	for (int i = octal.size() - 1; i >= 0; i -= 1)
	{
		d = octal[i] - 48;
		if (d < 0 || d > 7) error("Octal: out of range, bad octal.\n");
		d = d * pow(base, power);
		++power;
		result += d;
	}
	return result;
}
//-------------------------------------------------------------------------------------------------------------------------------

double Numerical_sysem::hexa_to_decimal(string hexa) 
{
	//works with positive rational numbers
	//the double is for the big numbers
	if (hexa.size() == 1 && hexa == "0") return 0;
	struct Hexadecimal {
		int what_is(char c) {
			switch (c)
			{
			case '0': case '1': case '2': case '3':	case '4':
			case '5': case '6': case '7': case '8': case '9':
				//cin.putback('\n'); cin.putback(c); cin >> digit;
				return c - 48;
			case'a':case 'A': return 10;
			case'b':case 'B': return 11;
			case'c':case 'C': return 12;
			case'd':case 'D': return 13;
			case'e':case 'E': return 14;
			case'f':case 'F': return 15;
			default:
				//	cin.putback(c); cin >> digit;
				error("Hexadecimal get: bad hexa.");
			}
		}
	};
	Hexadecimal hex;
	double result = 0;
	int power = 0;
	double digit;
	const int base = 16;
	for (int i = hexa.size() - 1; i >= 0; i -= 1)
	{
		digit = hex.what_is(hexa[i]);
		digit = digit * pow(base, power);
		++power;
		result += digit;
	}


	return result;
}
//-------------------------------------------------------------------------------------------------------------------------------

double Numerical_sysem::binary_to_decimal(string binary)
{
	//works with positive rational numbers
	//the double is for the big numbers
	double d;
	double result = 0;
	double power = 0;
	const int base = 2;
	for (int i = binary.size() - 1; i >= 0; i -= 1)
	{

		d = binary[i] - 48;
		if (d < 0 || d > 1) error("Binary: out of range, bad binary.\n");

		d = d * pow(base, power);
		++power;
		result += d;
	}
	return result;
}
//-------------------------------------------------------------------------------------------------------------------------------
string Numerical_sysem::decimal_to_hexa(double decimal) 
	//works with positive rational numbers
	//the double is for the big numbers
{
	narrow_cast<int>(fmod(decimal, 16));//is it a rational number
	if (decimal == 0) return "0";
	if (decimal < 0) error("decimal to hexa: out of range.\n");
	struct Hexadecimal {
		char what_is(int c) {
			char digit;
			switch (c)
			{
			case 0:case 1:case 2:case 3:case 4:
			case 5:case 6:case 7:case 8:case 9:
				digit = c + 48;
				return digit;
			case 10: return 'A';
			case 11: return 'B';
			case 12: return 'C';
			case 13: return 'D';
			case 14: return 'E';
			case 15: return 'F';
			default:
				error("Hexa get: Bad hexa.");
			}
		}
	};
	int base = 16; // base == 16
	double O = decimal;
	string hexa;
	Hexadecimal H;
	double L = O;
	hexa += H.what_is(fmod(L, base)); //1)    int 0-15
	L -= fmod(L, base);

	for (int p = 1; L != 0; ++p) {

		O = L / pow(base, p);//2)
		if (O >= base)
			O = fmod(O, base);
		hexa += H.what_is(int(O));

		L -= pow(base, p) * O; //3)
	}
	return revers_string(hexa);
}
//-------------------------------------------------------------------------------------------------------------------------------

string Numerical_sysem::decimal_to_octal(double decimal) 
{
	//works with positive rational numbers
	//the double is for the big numbers
	narrow_cast<int>(fmod(decimal, 16));//is it a rational number
	if (decimal < 0) error("decimal_to_octal: negative value.");
	double base = 8; // base == 8
	double O = decimal;
	string octa;
	double L = O; //what reminded
	octa += char(fmod(L, base)) + 48; //1)
	L -= fmod(L, base);

	for (int p = 1; L != 0; ++p) {

		O = L / pow(base, p);//2)
		if (O >= base)
			O = fmod(O, base);
		if (O > 8) error("decimal_to_octal: out of range.");
		octa += char(O) + 48;

		L -= pow(base, p) * O; //3)
	}
	octa = revers_string(octa);
	return octa;
}
//-------------------------------------------------------------------------------------------------------------------------------

string Numerical_sysem::decimal_to_binary(double decimal) 
{
	//works with positive rational numbers
	//the double is for the big numbers
	narrow_cast<int>(fmod(decimal, 16)); //is it a rational number

	if (decimal == 0) return "0";
	if (decimal < 0) error("decimal_to_binary: num > 0");
	string binary;
	const int base = 2; // base 2
	double N = 1;
	double size = 1;

	while (N * base <= decimal) { // once exeed we save our N(drom prev.loop and size and add '1'
		++size;
		N *= base;
	}
	if (N <= decimal) binary += '1';
	double E = N;
	while (N != decimal) {
		// we need and have: 1) size, 2) first binary digit, and 3) value represented as N
		while (decimal < (N + E / base)) {
			binary += '0';
			E /= base;
		}
		binary += '1';
		E /= base;
		N += E;
	}
	if (N > decimal) error("decimal out of range.");

	while (binary.size() < size) binary += '0';
	return binary;
}
//-------------------------------------------------------------------------------------------------------------------------------
