/*

4/23/2022 negative numeral conversions added
tested from -2000 to 2000
*/

#include "Custom Header Files/Numeral.h"
#include "constants.cpp"
#include "Custom Header Files/Other.h"
#include "Custom Header Files/roman_int.h"

namespace Numeral_system {
	//-------------------------------------------------------------------------------------------------------------------------------
	string get_numeral_from_decimal(int dec, Token_stream& ts)
	{
		//works with rational positive numbers
		//the double for big numbers
		char key{ ts.get().kind };
		switch (key)
		{
		case hexadecimal:
			return decimal_to_hexa(dec);
		case octal:
			return decimal_to_octal(dec);
		case binary:
			return decimal_to_binary(dec);
		case roman_int:
			int_to_roman(dec);
		default:
			ts.putback(key);
			error("numeral system key expected");
			break;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	string read_numeral(Token_stream& is)
	{
		char nl;
		string numeral;
		while (is.input_stream().get(nl) && is_space(nl) && nl != print);

		if (!(is_latin(nl) || is_digit(nl))) { if (!is.input_stream().eof()) is.putback_in_istream(nl); error("numeral expected"); }

		numeral += uc(nl);
		while (is.input_stream().get(nl) && is_latin(nl) || is_digit(nl)) numeral += uc(nl);
		if (!is.input_stream().eof()) is.putback_in_istream(nl);

		return numeral;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	int get_decimal_from_numeral(const char& key, Token_stream& is)
	{
		//works with rational positive numbers
		//the double for big numbers

		string numeral = read_numeral(is);

		switch (key)
		{
		case binary:
			return binary_to_decimal(numeral);
		case hexadecimal:
			return hexa_to_decimal(numeral);
		case octal:
			return octal_to_decimal(numeral);
		case roman_int:
			return roman_to_int(numeral);
		default:
			error("get: Numeral_system"); //key is checked in primary(); so this error is unlikely
			break;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	int octal_to_decimal(string octal)
	{
		const int base = 8;
		if (octal[0] == '7' && octal.size() != 1) { // negative
			int decimal{};
			int position{ int(octal.size()) };
			int o{};
			for (o; o != octal.size() - 1; ++o) { // always save last simbil
				if (octal[o] != '7') break;
				if (octal[o] == '7') octal[o] = '0';
				--position; // left to right
			}
			decimal = -narrow_cast<int>(pow(2, position * 3)); // max number that can be expressed up to the current octal simbol position. 3 means 3 bits for a hex single digit. 
			//int decimal{ -int(pow(2,int(hexa.size() * 4))) / 2 };
			position = 1;
			for (int i = int(octal.size()) - 1; i >= 0; i -= 1)
			{
				decimal += narrow_cast<int>((octal[i] - '0') * position);
				position *= base; // * 8 for each 3 bit in binary representation 
			}
			return decimal;
		}
		//--------------------------------------------
		int d;
		int result = 0;
		int power = 0;
		for (int i = int(octal.size()) - 1; i >= 0; i -= 1)
		{
			d = octal[i] - 48;
			if (d < 0 || d > 7) error("bad octal. Out of range");
			d = d * narrow_cast<int>(pow(base, power));
			++power;
			result += d;
		}
		return result;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	char oct_from_bin(const string& triplet)
	{
		if ("000" == triplet) return '0';
		if ("001" == triplet) return '1';
		if ("010" == triplet) return '2';
		if ("011" == triplet) return '3';
		if ("100" == triplet) return '4';
		if ("101" == triplet) return '5';
		if ("110" == triplet) return '6';
		if ("111" == triplet) return '7';
		error("bad binary triplet - ", triplet);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	string decimal_to_octal(int decimal)
	{
		if (decimal < 0) {
			string binary{ decimal_to_binary(decimal) };
			binary = revers_string(binary);
			while (binary.size() % 3 != 0) binary += '1'; // make 111111 from 1111. 
			binary = revers_string(binary);
			string three_bit;
			string oct;
			for (int b{}; b != binary.size(); )
			{
				for (int n{}; n != 3; ++n) // make three_bit
				{
					three_bit += binary[b];
					++b;
				}

				oct += oct_from_bin(three_bit);
				three_bit = {};
			}
			if (oct.size() % 3 != 0 || oct[0] != '7') { //  007 or 5000
				oct = revers_string(oct);
				while (oct.size() % 3 != 0 || oct[oct.size() - 1] != '7') oct += '7'; // make 7702 from 02. makes negative
				oct = revers_string(oct);
			}
			return oct;

		}

		const int base = 8; // base == 8
		int O = decimal;
		string octa;
		int L = O; //what reminded
		octa += char(fmod(L, base)) + 48; //1)
		L -= fmod(L, base);

		for (int p = 1; L != 0; ++p) {

			O = L / pow(base, p);//2)
			if (O >= base)
				O = fmod(O, base);
			if (O > 8) error("decimal_to_octal: out of range");
			octa += char(O) + 48;

			L -= pow(base, p) * O; //3)
		}
		while (octa.size() % 3 != 0 || octa[octa.size() - 1] == '7') octa += '0'; // make 077 from 77. makes positive

		octa = revers_string(octa);
		return octa;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	//hex-list
	int what_is(const char& c) {
		switch (c)
		{
		case '0': case '1': case '2': case '3':	case '4':
		case '5': case '6': case '7': case '8': case '9':
			return c - 48;
		case'a':case 'A': return 10;
		case'b':case 'B': return 11;
		case'c':case 'C': return 12;
		case'd':case 'D': return 13;
		case'e':case 'E': return 14;
		case'f':case 'F': return 15;
		default:
			error("Hexadecimal get: bad hexa - ", string{c});
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	int hexa_to_decimal(string hexa)
	{
		const int base = 16;
		if (hexa[0] == 'F' && hexa.size() != 1) { // negative
			int decimal{};
			int position{ int(hexa.size() )};
			int h{};
			for (h; h != hexa.size()-1;++h) { // always save last simbil
				if (hexa[h] != 'F') break;
				if (hexa[h] == 'F') hexa[h] = '0';
				--position; // left to right
			}
			decimal = -narrow_cast<int>(pow(2, position * 4)); // max number that can be expressed up to the current hex simbol position. 4 means 4 bits for a hex single digit. 
			//int decimal{ -int(pow(2,int(hexa.size() * 4))) / 2 };
			position = 1;
			for (int i = int(hexa.size()) - 1; i >= 0; i -= 1)
			{
				decimal += narrow_cast<int>(what_is(hexa[i]) * position);
				position *= base; // * 16 for each 4 bit quartet in binary representation 
			}
			return decimal;
		}
		//--------------------------------------------
		int result = 0;
		int power = 0;
		int digit;
		for (int i = int(hexa.size()) - 1; i >= 0; i -= 1)
		{
			digit = what_is(hexa[i]);
			digit = digit * int(pow(base, power));
			++power;
			result += digit;
		}
		return result;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	//hex-list
	char what_is(const int& c)
	{
		switch (c)
		{
		case 0:case 1:case 2:case 3:case 4:
		case 5:case 6:case 7:case 8:case 9:
			return c + 48;
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
	//-------------------------------------------------------------------------------------------------------------------------------
	char hex_from_bin(const string& quartet)
	{
		if ("0000" == quartet) return '0';
		if ("0001" == quartet) return '1';
		if ("0010" == quartet) return '2';
		if ("0011" == quartet) return '3';
		if ("0100" == quartet) return '4';
		if ("0101" == quartet) return '5';
		if ("0110" == quartet) return '6';
		if ("0111" == quartet) return '7';
		if ("1000" == quartet) return '8';
		if ("1001" == quartet) return '9';
		if ("1010" == quartet) return 'A';
		if ("1011" == quartet) return 'B';
		if ("1100" == quartet) return 'C';
		if ("1101" == quartet) return 'D';
		if ("1110" == quartet) return 'E';
		if ("1111" == quartet) return 'F';
		error("bad binary quartet - ", quartet);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	string decimal_to_hexa(int decimal)
	{
		if (decimal < 0) {
			string binary{ decimal_to_binary(decimal )};
			string hex{};
			string quartet_bin;
			for (int b{}; b != binary.size(); )
			{
				for (int n{}; n != 4; ++n) // make quartet_bin
				{
					quartet_bin += binary[b];
					++b;
				}

				hex += hex_from_bin(quartet_bin);
				quartet_bin = {};
			}
			if (hex.size() % 4 != 0 || hex[0] != 'F') { //  00A or A000
				hex = revers_string(hex);
				while (hex.size() % 4 != 0 || hex[hex.size() - 1] != 'F') hex += 'F'; // make FF02 from 02. makes negative
				hex = revers_string(hex);
			}
			return hex;
		}
		//-------------------------------------------
	//	narrow_cast<int>(fmod(decimal, 16));//is it a rational number
		if (decimal == 0) return "0000";

		const int base = 16; // base == 16
		int O = decimal;
		string hexa;

		int L = O;
		hexa += what_is(int(fmod(L, base))); //1)    int 0-15
		L -= int(fmod(L, base));

		for (int p = 1; L != 0; ++p) {

			O = L / int(pow(base, p));//2)
			if (O >= base)
				O = int(fmod(O, base));
			hexa += what_is(O);

			L -= int(pow(base, p)) * O; //3)
		}
		
		while (hexa.size() % 4 != 0 || hexa[hexa.size()-1] == 'F') hexa += '0'; // make 00FF from FF. makes positive
		
		return revers_string(hexa);
	}
	
	//-------------------------------------------------------------------------------------------------------------------------------
	int binary_to_decimal(string binary)
	{
		int d{};
		int result = 0;
		const int base = 2;

		if (binary[0] == '1' && binary.size() != 1) {
			binary[0] = '0';
			result = narrow_cast<int>(pow(base, binary.size() - 1)) * -1;
		}

		int power = 0;
		for (int i = int(binary.size()) - 1; i >= 0; i -= 1)
		{
			d = binary[i] - 48;
			if (d < 0 || d > 1) error("Bad binary. Out of range", d);

			d = d * narrow_cast<int>(pow(base, power));
			++power;
			result += d;
		}
		return result;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	string decimal_to_binary(int decimal)
	{
		if (decimal == 0) return "0000";
		if (decimal < 0)
		{
			string binary{ "1" }; // first 1 represents negative number
			int approximation = -8; // 1000 == - 8 is the max negative number for 4 bit
			int size = 4;  // 4 bit binary
			while (decimal < approximation) 
			{
				size += 4; // add 4 bit each turn
				approximation *= 16; // max negative number that can be represented with current number of bits
			}
			int increment{ approximation/-2 }; // each bit, left to right, adds int_previous_bit/2 to the negative number
			while (size != binary.size())
			{
				if (approximation + increment <= decimal) {
					binary += '1';
					approximation += increment;
				}
				else binary += '0'; 
				increment /= 2;
			}
			return binary;
		}
		//--------------------------------------------------------
		string binary{ "" };
		const int base = 2; // base 2
		int N = 1; // the number refference
		int size = 1;

		while (N * base <= decimal) { // once exceed we save our N from previous loop and size and then add '1'
			++size;
			N *= base;
		}
		if (N <= decimal) binary += '1';
		int E = N;
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
		if (N > decimal) error("decimal out of range");
		while (binary.size() < size) binary += '0'; // only 0 remained to the number
		
		if(binary.size() % 4 != 0 || binary[0] == '1') { //  001 or 1000
			binary = revers_string(binary);
			while (binary.size() % 4 != 0 || binary[binary.size() - 1] == '1') binary += '0'; // make 0011 from 11. makes positive
			binary = revers_string(binary);
		}
		return binary;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
}


/* test
	string g;
	for (int t{ 2000 }; t > -2000; --t) {
		g = Numeral_system::decimal_to_hexa(t) ;
		cout <<g << "==" << t << "==" << Numeral_system::hexa_to_decimal(g) << "==" 
			<<  Numeral_system::decimal_to_binary(t) << "==" 
			<< Numeral_system::binary_to_decimal (Numeral_system::decimal_to_binary(t))<< '\n';
	if ( t != Numeral_system::hexa_to_decimal(g) ||
		Numeral_system::binary_to_decimal(Numeral_system::decimal_to_binary(t)) != t||
		Numeral_system::binary_to_decimal(Numeral_system::decimal_to_binary(Numeral_system::hexa_to_decimal(g))) != t) error("error");
	}
	
	*/