

//Simple calculator

#include "Custom Header Files/std_lib_facilities.h"

//Numeral_sysem  Key words
static const string Binary_key{ "bin" }; // "bin"
static const string Hexa_key{ "hex" };  // "hex"
static const string Octa_key{ "oct" };// "oct"
static const string Dec_key{ "dec" }; // "dec"
static const string convert_key{ "convto" }; // "convto"
//---------------------------------------------------------------------
//roman_int Key word
static const string  roman_int_key = "rom";// "rom"
//---------------------------------------------------------------------

//Simbol table Key words
static const char decl_key = '#';        // '#' starts declaration
static const string assign_key = "let";  // "let "change variable
static const string const_decl_key = "const"; // "const" means declaration of the constant value
//---------------------------------------------------------------------

// other Key words
static const string exit_key = "quit"; // "quit"
static const string help_key = "help"; // "help"
static const string from_file_key = "from"; // "from"
static const string to_file_key = "to"; // "to"
static const string stop_file_key = "stopfile"; // "stopfile"

//---------------------------------------------------------------------

//Token representations
static const char number = '8';          //t.kind == number means number Token
static const char name = 'n';            //t.kind == name means name Token
static const char Exit = 'e';            //t.kind == exit means exit Token
static const char assign = 'a';          //t.kind == assign means assign Token
static const char constant = 'c';		  // represents a constant . 
static const char roman_int = 'R';
static const char hexadecimal = 'X';
static const char octal = 'O';
static const char decimal = 'D';
static const char binary = 'B';
static const char convert = 'c';
static const char help = 'h'; // help
static const char print = '\n'; // pressed enter
static const char from_file = 'f'; 
static const char to_file = 't'; // 't'
static const char stop_file = 's'; // 's'
//---------------------------------------------------------------------

//Element()'s calculation operators
static const char square_root = '\"'; // '"'
static const char power_of = '^'; // '^'
static const char factorial = '!'; // '!'
//---------------------------------------------------------------------

//Calculation prompts
static const string prompt = "> ";		  // "> " a promt for enter 
static const string result = "= ";		  // "= " what follows is a result of calculation
