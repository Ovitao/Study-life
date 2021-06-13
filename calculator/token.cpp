
/*

Simple calculator




*/
#include "Custom Header Files/token.h"
#include "constants.cpp"



//-----------------------------------------------------------------------------
// our main input intermediary
Token Token_stream::get() {
	if (full) {  // filled by ts.putback 
		full = false;
		return buffer;
	}
	char ch;

	cin.get(ch);
	while (islatin(ch) && isspace(ch) && ch != print) cin.get(ch);
	switch (ch) {
	case '(': 	case ')':
	case '+':	case '-':
	case '*':	case '/':
	case '%':	case '=':
	case square_root: case decl_key:
	case power_of:	case factorial:
	case print:
		return Token{ ch };	// each character represents itseif
	case '.':
		return is_there_digit(ch);
	case '0': case '1': case '2': case '3':	case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.unget();
		double val;
		cin >> val;
		return Token{ number, val };	// number Token
	}
	default:

		if (islatin(ch) && isalpha(ch)) return key_word(ch);

		cin.unget();
		error("Bad token");
	}
}
//---------------------------------------------------------------------

void Token_stream::putback(Token t)
{
	if (full) error("putbck: into a full buffer");
	buffer = t;
	full = true;
}

//---------------------------------------------------------------------
Token Token_stream::key_word(char ch) {
	string s;
	char c;
	s += ch;
	while (cin.get(c) && islatin(c) && isalpha(c)) s += c;
	cin.unget();
	//
	if (s == help_key) return Token(help); //  help_list();
	if (s == Hexa_key) return Token(hexadecimal); //  hexa_to_decimal();
	if (s == Binary_key) return Token(binary); //  binary_to_decimal();
	if (s == Octa_key) return Token(octal); //  octal_to_decimal();
	if (s == assign_key) return Token(assign); //  assignment();
	if (s == quit_key) return Token(quit); //  off the program.
	if (s == const_decl_key) return Token(constant); // used with decl_key
	if (s == convert_key) return Token(convert); // run get_decimal_from(); . include: decimal_to_octal(); decimal_to_hexa(); decimal_to_binary();
	//  
	while (cin.get(c) && islatin(c) && isalpha(c) || isdigit(c) || c == '_') s += c;
	cin.unget();
	return Token{ name, s };	 // name Token
}

//-----------------------------------------------------------------------------

// skip expression 
void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	char ch;
	while (cin.get(ch))
		if (ch == c) return;
	if (!cin) error("Can't recover from error. Bad input.");
}
//---------------------------------------------------------------------


//-----------------------------------------------------------------------------
Token_stream::Token_stream(istream&)
	:full{ false }, buffer{ 0 }
{
}
//-----------------------------------------------------------------------------
Token::Token()
	: kind{ 0 }, value{ 0 }
{
}

// to avoid a fatal error caused by some functions such as isdigit,isalpha,isspase etc. 
bool islatin(char r) {
	if (r >= -128 && r <= -1) error("Not latin.");
	return true;
}
//-----------------------------------------------------------------------------

// handle a fatal error: if only a char such as '.' to the double is entered
Token is_there_digit(char dot) {
	char c; cin.get(c);
	cin.putback(c);
	if (islatin(c) && isdigit(c)) {
		cin.putback(dot);
		double val; cin >> val;
		return Token{ number, val };	// number Token
	} 
	error("a digit expected after '.'");
}
//-----------------------------------------------------------------------------
