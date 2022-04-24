#include "std_lib_facilities.h"


//---------------------------------------------------------------------
class Token {
public:
	char kind;		// what kind of token - name:, number: etc.
	double value;
	string name;
	Token(char ch)					//  an operaor or a keyword
		:kind{ ch }, value{ 0 } {}
	Token(char ch, double val)		//  numbers
		:kind{ ch }, value{ val } {}
	Token(char ch, string name)		//  variables and constants
		:kind{ ch }, name{ name } {}
	Token();
};
//---------------------------------------------------------------------
class Token_stream { 
public:
	Token_stream (istream&);
	//also skips spaces
	Token get(); 
	void putback (const Token&); // to hold a Token
	void ignore (const char&); // skips expression until char
	istream& input_stream(); // ifstream or cin
	void putback_in_istream(const char&); // putback in the input_stream(), ifstream or cin
	Token key_word (char&); 
	const bool& full_buffer() { return full; };
	const bool& full_istream_buffer() { return full_istream; };
private:
	istream& in_stream;
	bool full_istream; // for prompt needance check in other function
	bool full; 
	Token buffer; // holds a Token
};
//---------------------------------------------------------------------

// handle error "a digit expected after point in double of type .1 .3".
Token is_there_digit(char point, istream&);
