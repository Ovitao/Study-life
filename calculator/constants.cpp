

//Simple calculator

#include "Costom Header Files/std_lib_facilities.h"

//Numerical_sysem  Key words
static const string Binary_key{ "bin" }; //"bin"
static const string Hexa_key = "hex";  // "hex"
static const string Octa_key = "oct";//"oct"
static const string Dec_key = "dec"; //"dec"
static const string convert_key = "to"; //"to"
//---------------------------------------------------------------------

//Simbol table Key words
static const char decl_key = '#';        // # starts declaration
static const string assign_key = "let";  // change variable
static const string const_decl_key = "const"; // 'const' means declaration of the constant value
//---------------------------------------------------------------------

// other Key words
static const string quit_key = "exit"; //"exit"
static const string help_key = "help";



//---------------------------------------------------------------------

//Token representations
static const char number = '8';          //t.kind == number means number Token
static const char name = 'n';            //t.kind == name means name Token
static const char quit = 'q';            //t.kind == quit means quit Token
static const char assign = 'a';          //t.kind == assign means assign Token
static const char constant = 'c';		  // represents a constant . 
static const char hexadecimal = 'X';
static const char octal = 'O';
static const char decimal = 'D';
static const char binary = 'B';
static const char convert = 't';
static const char help = 'h'; //help
static const char print = '\n'; // press enter
//---------------------------------------------------------------------

//Element()'s calculation operators
static const char square_root = '"';
static const char power_of = '^';
static const char factorial = '!';
//---------------------------------------------------------------------

//Calculation prompts
static const string prompt = "> ";		  //a promt for enter 
static const string result = "= ";		  //what follows is a result of calculation
