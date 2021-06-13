
/*
	 Simple calculator

	Revision history:
		Revised by Tao June 2021 
							Roman_int added

		Revised by Tao October 2020
		
	
	Modifications: Binary sistem has set up
	
			
			This program implements a basic expression calculator.
			Variables and Constants are implemented.

			The grammar for input is:
--------------------------------------
Calculation:
	Statement print
	Calculation Statement print
	Calculation quit
	Calculation "convert_key" num_system_key

	print
	quit

Statement:
	Declaration 
	Assignment
	Expression 

Declaration:
	decl_key		'name' "=" Expression
	decl_key  const	'name' "=" Expression

Assignment:
	"let" name "=" Expression

Expression:
	Term
	Expression '+' Term
	Expression '-' Term
	
Term:
	Element
	Term '*' Element
	Term '/' Element

Element:
	Primary
	Primary '!'
	sqrt Primary
	Primary '^' Primary

Primary:
	Number
	Simbol
	num_system_key literal
	Romain_int_key literal
	'-' Primary
	'+' Primary
	'(' Expression ')'

Simbol:
	Variable
	Constant

Number:
	floating-point-litteral
	
Num_system literal
	Binary_key
	Hexadecimal_key
	Octal_key

Romain int literal
	Romain_int_key (rom)
--------------------------------------



	input from cin. through the "Token_stream" called "ts".
	Variables and Constants implemented by "Simbol_table" called "st".



	The Num_system doesn't have negative numbers yet
	doesn't handled double simbols and overflow. 

	help text a little need rewrite somewhere

*/

#include "Custom Header Files/std_lib_facilities.h"
#include "Custom Header Files/token.h"
#include "Custom Header Files/Simbol.h"
#include "Custom Header Files/Numeric.h"
#include "constants.cpp"
//---------------------------------------------------------------------

Simbol s;

Simbol_table st;

Numerical_sysem ns;

//---------------------------------------------------------------------
//recover from error
void clean_up_mess(Token_stream& ts){
	ts.ignore(print);
}

double expression(Token_stream&);
double ft(double );

//---------------------------------------------------------------------

//handle '(' ')', +primary, -primary, Simbol, 
double primary(Token_stream& ts)
{
	Token t = ts.get();
	switch (t.kind) {
	case '(': 
	{	
		double d = expression(ts);
		t = ts.get();
		if (t.kind != ')') { cin.unget(); error("')' expected"); }
		return d;
	}
	case '-':
		return - primary(ts);
	case '+':
		return   primary(ts);
	case number: 
		return t.value;
	case name: //varibles and constants
		return st.get(t.name);
	case hexadecimal:
	case binary:
	case octal:
		return ns.get_decimal_from_literal(t.kind);
	default: 
		cin.unget();
		error("primary expected");
	}
}
//---------------------------------------------------------------------
// handle square root, to the power of, factorial, 
double element(Token_stream& ts)
 {
	double left;
	Token t = ts.get();							//because square_root_key goes before a digit
	if (t.kind == square_root) left = primary(ts);
	else {
		ts.putback(t);
		left = primary(ts);
		t = ts.get();
	}

	switch (t.kind)	{
		case factorial:
			return ft(left);
		case power_of: {
			int p = narrow_cast<int>(primary(ts)); // p = integer. originaly in this program
			return pow(left, p);
		}
		case square_root:
			if (left < 0)
				error("square root of negative number", left);
			return sqrt(left);
		default:
			ts.putback(t);
			return left;
	}
}
//---------------------------------------------------------------------
//handle '%', '/', '*'
double term(Token_stream& ts)
{
	double left = element(ts);

	while (true) {
		Token t = ts.get();

		switch (t.kind) {
		case '*':
			left *= element(ts);
			break;
		case '/':
		{	
			double d = element(ts);
			if (d == 0) error("divide by zero");
			left /= d;
		}
		break;
		case '%':
		{	
			double d = element(ts);
			if (d == 0) error("divide by zero");
			left = fmod(left, d);
		}
		break;
		default:
			ts.putback(t);
			return left;
		}
		
	}
}
//---------------------------------------------------------------------
//handle '+' '-' , 
double expression(Token_stream& ts)
{
	double left = term(ts);
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		
		case '+':
			left += term(ts);
			break;
		case '-':
			left -= term(ts);
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}
//---------------------------------------------------------------------





double assignment(Token_stream& ts);	double declaration(Token_stream& ts); 

void clean_up_mess(Token_stream& ts);	void help_list();





//---------------------------------------------------------------------
double statement(Token_stream& ts)
{
	while (true) 	{
		Token t = ts.get();
		switch (t.kind) 
		{
		case decl_key:
			return declaration(ts);
		case assign:
			return assignment(ts);
		default:
			ts.putback(t);
			return expression(ts);
		}
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//main eValuation loop
void calculate(Token_stream& ts) {
	double d;
	while (true) 
	try {
		cout << prompt;
		Token t = ts.get();

		while (t.kind == print || t.kind == help) {		//first skip all "print" and handle all "help"
			if (t.kind == help) {
				help_list();
				cout << prompt;
			}  
			t = ts.get();
		}
		if (t.kind == quit) return;
		ts.putback(t);
		d =  statement(ts); // evaluate

		t = ts.get(); 
		if (t.kind == convert) //output for Numerical conversion 
			cout << result << ns.get_literal_from_decimal(d) << endl;
						else{ //output as usual 
							ts.putback(t);
							cout << result << d << endl; }
	}
	catch (exception& e) {
		cerr << "error: " << e.what() << endl;
		clean_up_mess(ts);
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------




double assignment(Token_stream& ts)
// assume we've seen assignment key (let)
// handle name '=' expression();
// assign value to an existing Variable. 
{
	Token t = ts.get();
	
	if (t.kind != name) {
		cin.unget(); error("name expected in assignment");
	}

	string varname = t.name;
	if (!st.is_declared(varname))	error(varname, " not declared");
	
	Token t2 = ts.get();
	if (t2.kind != '=') { cin.unget(); error("= missing in declaration of ", varname); }

	double d = expression(ts);

	return st.set(varname, d);
}
//---------------------------------------------------------------------
double declaration(Token_stream& ts)
// assume: we've seen declaration key
// handle: name '=' expression();
// create: Variable with a name and a value.
{
	Token t = ts.get();

		if (t.kind == constant) {
			t = ts.get();
			s.constant = true;
		} else s.constant = false;

		if (t.kind != name) {
			cin.unget();
			error("name expected in declaration");
		}

	string var = t.name;
		if (st.is_declared(var)) 	error(var, " already declared");
		
	
	Token t2 = ts.get();
		if (t2.kind != '=') {
			cin.unget();
			error("= missing in declaration of ", var);
		}
	
	double d = expression(ts);
		if (s.constant)
		return st.declare(var, d, constant);
		return st.declare(var, d );
}
//---------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
 void intro();
// scarfolding
int main()
try {
	
	Token_stream ts (cin);
	intro();
	
	//------===========================

	st.declare("pi", 3.1415926535, constant);
	st.declare("e", 2.7182818284, constant);
	
	//st.fill_table();
	
	
	//------===========================
	
	calculate(ts);
	keep_window_open();  //cope with windows console mode
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	keep_window_open("~1");
	return 1;
}
catch (...) {
	cerr << "unexcpected failure\n";
	keep_window_open("~3");
	return 3;
}
//---------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//factorial (incledes assertion on positive number)
double ft(double n) {
	if (n == 0|| n == 1) return 1;
	if (n < 0) error("factorial with negative number");
	double num = int(n);
	if ( n - num  >= .00001)  error("factorial: integers are only allowed.");

	for (double mult = num - 1; mult > 1; mult -= 1)
		num *= mult;
	return num;
}
//---------------------------------------------------------------------










string line(int length) {
	string s;
	for (int i = 0; i < length; ++i) s += '-';
	s += '\n';
	return s;
}

void help_list() {
	char ch;
	cout << " \n\n= What you would like to know of = \n"
		<< " 1. Variables and constants. \n"
		<< " 2. Operators. \n"
		<< " 3. Print Variables. \n"
		<< " 4. Print Constants. \n"
		<< " 5. Numerical system. \n"
		<< "'q' for quit \n";
	while (true) {
		cin >> ch;
		switch (ch) {
		case '1':
			cout << '\n';
			cout << line(40);
			cout << " - Declare a new variables: #n=(expression).  i.e. #n=5-2^2 \n"
				<< " - Use 'const' to declare constants: #const n=(expression)\n"
				<< " - Change a variable: let n=(expression)\n"
				<< " - Use them in expressions by typing their names: (a+b)*a  \n"
				<< " A-z_0-9 for names. Start it with A-z\n";
			cout << line(40);
			break;
		case '2':
			cout << '\n';
			cout << line(40);
			cout << " Available orepators: \n"
				<< "\t () \n"
				<< "\t + " << " - \n"
				<< "\t / " << " * \n"
				<< "\t % \n"
				<< "\t \"       :square root. \"n" << '\n'
				<< "\t !       :factorial. n!" << '\n'
				<< "\t ^       :to the power of. n^n" << '\n';
			cout << line(40);
			break;
		case '3':
		{
			cout << line(15);
			cout << "  Variables:" << '\n';
			for (Simbol s : st.table) {
				if (!s.constant) cout << s.name << "= " << s.value << '\n';
			}
			cout << line(15);
		}
		break;
		case '4':
		{
			cout << line(15);
			cout << "  Constants:" << '\n';
			for (Simbol s : st.table) {
				if (s.constant) cout << s.name << "= " << s.value << '\n';
			}
			cout << line(15);
		}
		break;
		case '5':
			cout << '\n';
			cout << line(40);
			cout << "So far. works only with rational positive numbers.\n"
				<< "Default output goes as decimal.\n"
				 << "To convert use \"to\". e.g. hex 8f to bin." << '\n'
				<< "You may use expressions as well.\n"
				 << "\t bin       :introduses binary literal " << '\n'
				 << "\t hex       :introduses hexadecimal literal " << '\n'
				 << "\t oct       :introduses octal literal " << '\n';
			cout << line(40);
			break;
		case 'q':
			return;
		default:
			 cout << " \n\n= What you would know of = \n"
				<< " 1. Variables and constants. \n"
				<< " 2. Operators. \n"
				<< " 3. Print Variables. \n"
				<< " 4. Print Constants. \n"
				<< "'q' for quit \n";
			break;
		}

	}
}



//Welcom to our simple calculator
 void intro() {
	cout << "Welcom to our simple calculator. \n"
		<< "Enter expression using floating-point number, end it with pressing enter. \n"
		<< "\"help\" for help. \n"
		<< "\"quit\" for quit. \n"
		;
}
