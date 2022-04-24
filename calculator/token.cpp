
/*

Simple calculator




*/
#include "Custom Header Files/token.h"
#include "Custom Header Files/Other.h"
#include "constants.cpp"



//-----------------------------------------------------------------------------
Token_stream::Token_stream(istream& i)
	:full_istream{ false }, full{ false }, buffer{ 0 }, in_stream{ i }
{
}
//-----------------------------------------------------------------------------
Token::Token()
	: kind{ 0 }, value{ 0 }
{
}
istream& Token_stream::input_stream() 
{ full_istream = false; return in_stream; }

//-----------------------------------------------------------------------------
// our main input intermediary
Token Token_stream::get() 
{
	if (full) {  // filled by ts.putback()
		full = false;
		return buffer;
	}
	
	char ch;
	full_istream = false;
	while (in_stream.get(ch) && is_space(ch) && ch != print); // pass spaces

	switch (ch) 
	{
		case '(': 	case ')':
		case '+':	case '-':
		case '*':	case '/':
		case '%':	case '=':
		case square_root: case decl_key:
		case power_of:	case factorial:
		case print:
			return Token{ ch };	// each character represents itseif
		case '.':
			return is_there_digit(ch, in_stream);
		case '0': case '1': case '2': case '3':	case '4':
		case '5': case '6': case '7': case '8': case '9':
		{
			in_stream.unget();
			double val;
			in_stream >> val;
			return Token{ number, val };	// number Token
		}
		default:
		{
			if (is_latin(ch)) return key_word(ch);

			if (in_stream.eof()) return Token(); // let the other part of the program handle this
			
			full_istream = true;
			in_stream.unget();
			error("Bad token - ", string{ ch });
		}
	}
}
//---------------------------------------------------------------------
Token Token_stream::key_word(char& c) 
{
	string s{ c };
	
	//key_word
	while (in_stream.get(c) && is_latin(c)) s += c;
	if (!in_stream.eof()) { full_istream = true; in_stream.unget(); }

	if (s == help_key) return Token(help); //  help_list();
	if (s == Hexa_key) return Token(hexadecimal); //  hexa_to_decimal();
	if (s == Binary_key) return Token(binary); //  binary_to_decimal();
	if (s == Octa_key) return Token(octal); //  octal_to_decimal();
	if (s == assign_key) return Token(assign); //  assignment();
	if (s == exit_key) return Token(Exit); //  off the program.
	if (s == const_decl_key) return Token(constant); // used with decl_key
	if (s == convert_key) return Token(convert); // run get_decimal_from(); . include: decimal_to_octal(); decimal_to_hexa(); decimal_to_binary();
	if (s == from_file_key) return Token(from_file);
	if (s == to_file_key) return Token(to_file);
	if (s == stop_file_key) return Token(stop_file);
	if (s == roman_int_key) return Token(roman_int);
												 //  
	//variable or constant
	while (in_stream.get(c) && is_latin(c) || is_digit(c) || c == '_') s += c;
	if (!in_stream.eof()) in_stream.unget();

	return Token{ name, s };	 // name Token
}
//-----------------------------------------------------------------------------
// skips expression 
void Token_stream::ignore(const char& c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	char ch;

	full_istream = false;
	while (in_stream.get(ch))
		if (ch == c) return;
	if (in_stream.eof()) return; // end of file, move forward. (to another file or cin)
	if (!in_stream) error("Can't recover from error. Bad input");
}
//---------------------------------------------------------------------
void Token_stream::putback(const Token& t)
{
	if (full) error("putback: into full buffer");
	buffer = t;
	full = true;	
}
//-----------------------------------------------------------------------------
// handle the fail(): if only a char such as '.' to a double is entered
// for .1 .02 number representstions
Token is_there_digit(char point, istream& is) 
{
	char c; is.get(c);
	is.putback(c);
	if (is_digit(c)) {
		is.putback(point);
		double val; is >> val;
		return Token{ number, val };	// number Token
	} 
	error("a digit expected after '.'");
}
//-----------------------------------------------------------------------------
void Token_stream::putback_in_istream(const char& ch)
{
	full_istream = true;
	in_stream.putback(ch);
}