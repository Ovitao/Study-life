


#include "std_lib_facilities.h"




class Token {
public:
	char kind;		// what kind of token
	double value;
	string name;
	Token(char ch)					//  an operaor or keyword
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
	Token get(); 
	void putback (Token ); // hold Token
	void ignore (char ); // skip expression
	Token key_word (char ); 
private:
	bool full; 
	Token buffer; // hold Token
};
//---------------------------------------------------------------------

bool islatin(char r);
//handle input format error
Token is_there_digit(char ch);
//low case literal
char lc(char c);