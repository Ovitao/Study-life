
/*
	 Simple calculator

	Revision history:
		Revised by Tao April 2022
								file streams added
								roman_int added
								bugs fixed
								little text changes
								some minor changes
								element() reworked
								negative numbers representations for Numeral system added 
		Revised by Tao June 2021 
								Element added
								constants added
								Assignment added
								Numeral system added
								minor changes
		Revised by Tao October 2020
								help section added
								key_word added
								remainder added
			
--------------------------------------
The grammar for input is:

streams switch:
	Calculation
	read_file
	set_ofsteram
	stopfile_key

read_file:
	Calculation
	from_file_key file_name
	to_file_key file_name
	read_file read_file

Calculation:
	Statement print
	Calculation Statement print
	Calculation quit
	Calculation "convert_key" num_system_key

	stopfile_key
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
	Element'!'
	sqrt_key Element
	Element '^' Element

Primary:
	Number
	Simbol
	num_system_key literal
	Romain_int_key literal
	'-' Primary
	'+' Primary
	'(' Expression ')'

Simbol:
	Variable_name
	Constant_name

Number:
	floating-point-litteral

Numeral system
	Binary_key string-numeral
	Hexadecimal_key string-numeral
	Octal_key string-numeral
	Roman int

Roman int
	roman_int_key string-numeral
--------------------------------------


	input from cin or filestream. through the "Token_stream".
	Variables and Constants implemented by "Simbol_table" called "st".


	The Numeral system doesn't have negative numbers yet
*/
#include "Custom Header Files/roman_int.h"
#include "Custom Header Files/std_lib_facilities.h"
//#include "Custom Header Files/token.h"
#include "Custom Header Files/Simbol.h"
#include "Custom Header Files/Numeral.h" // + token.h
#include "Custom Header Files/file_stream.h"
#include "Custom Header Files/Other.h"
#include "constants.cpp"
//---------------------------------------------------------------------

Simbol_table st;

//---------------------------------------------------------------------
//recover from error: pass characters until 'print'
void clean_up_mess(Token_stream& ts){
	ts.ignore(print);
}

double expression(Token_stream&);

//---------------------------------------------------------------------

//handle '('expression')', +number, -number, Simbol, numeral
double primary(Token_stream& ts)
{
	Token t = ts.get();
	
	switch (t.kind) {
	case '(': 
	{	
		double d = expression(ts);
		t = ts.get();
		if (t.kind != ')') { ts.putback(t); error("')' expected"); }
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
	case hexadecimal: case binary: case octal: case roman_int: // convert to decimal and proceed
		return Numeral_system::get_decimal_from_numeral(t.kind, ts);
	default: 
		ts.putback(t);
		error("primary expected");
	}
}
//---------------------------------------------------------------------
// handle square root "n, to the power of n^n, factorial n!, 
double element(Token_stream& ts)
{
	double n{ 1 };
		//----
	Token t = ts.get();	
	ts.putback(t);
	
	if (t.kind != square_root) n = primary(ts); // due to the square_root_key goes before a digit		
	
	while (true) 
	{
		t = ts.get();

		switch (t.kind) {
		case factorial:
			n = ft(n); // works with n!!!
			break;
		case power_of:
			return pow(n, element(ts)); // works with n^n^n == n^(n^n)
		case square_root: {
			double p = element(ts); // to check negative value
			if (p < 0)
				error("square root of negative number", int(p)); // no error() for double
			n *= sqrt(p); // works with ""n == "("n) and "n"n == "(n * "n)
		}
		break;
		default:
			ts.putback(t);
			return n;
		}
	}
}
//---------------------------------------------------------------------
//handle '%', '/', '*'
double term(Token_stream& ts)
{
	double left = element(ts);
	while (true)
	{
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
			if (d - long long int(d) != 0)  error("remainder: only integers are allowed");
			left = fmod(left, int(d));
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
	double left = term(ts); // left hand side of the equation, term() is on the right
	while (true) {
		Token t = ts.get();
		switch (t.kind) 
		{
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

double assignment(Token_stream&);	double declaration(Token_stream&); 

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
//---------------------------------------------------------------------


void clean_up_mess(Token_stream&);	void help_list(Token_stream&, ostream&); void helper(Token_stream&, ostream&);

string& prompt_print(Token_stream&); void disable_prompt(); void activate_prompt(); 


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

//begin-calculate-print
void calculate(Token_stream& ts, ostream& os) 
{
	while (true) 
	try {
		helper(ts, os);	//first skip all "print/space" and handle all 'help' and print 'prompt'

		Token t = ts.get();  
		ts.putback(t);
		
		if (ts.input_stream().fail() || t.kind == from_file
			|| t.kind == to_file || t.kind == stop_file || t.kind == Exit) return;
		
		double d = statement(ts); // all calculations

		t = ts.get(); 
		if (t.kind == convert) { //output for Numeral system conversion 
			if (d - long long int(d) != 0)  error("Numeral_system: only integers are allowed");
			os << result << Numeral_system::get_numeral_from_decimal(narrow_cast<int>(d), ts) << endl;}
						else { //output as usual
							ts.putback(t);
							os << result << d << endl; }
	}
	catch (exception& e) 
	{
		os << "Error: " << e.what() << '.' << endl;
		clean_up_mess(ts);
	}
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------


void set_ofstream(istream& name);

void read_file(Token_stream& ts);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// allows to use "from file" or "to file" within other files
void read_file(Token_stream& ts)
{
	static bool file{ false }; // is ofstream initialised
	string name{ read_file_name(ts.input_stream()) };
	ifstream ifs; // new ifstream for each from_file encountered in files
	Token_stream ts_file(initialize_ifstream(ifs, name));
	cout << "Open input file " << name << ". Reading started...\n";

	Token t = ts.get();
	if (t.kind == to_file) // "from file1" followed by "to file2" received from the same istream
		set_ofstream(ts.input_stream());
	else ts.putback(t); // continue the previous after the return
	t = ts_file.get();

	while (true)
	{
		if (t.kind == to_file) // to_file encountered in the new ifstream
			set_ofstream(ts_file.input_stream());
		else
		if (t.kind == from_file) // from_file encountered in the new ifstream
			read_file(ts_file);
		else
		if (t.kind == stop_file) {
			cout << "Stopped input file ";
			ts.get(); // clear the buffer. because  we stop all file readings  now
			ts.putback(t);
			break;
		}
		else
		if (ts_file.input_stream().eof()) {
			cout << "End of input file ";
			break;
		}
		else {
			ostream& os = fetch_ostream(); // get ofstream if open or cout
			ts_file.putback(t);
			calculate(ts_file, os); // calculate contents of the file
		}
		t = ts_file.get();
	}
	cout << name << ". Reading finished." << endl;
	return;
}
//-----------------------------------------------------------------------------
void set_ofstream(istream& name_stream)
{
	string& name = ofs_name();
	close_ofstream();
	name = { read_file_name(name_stream) };
	initialize_ofstream(name); // ofsteam is static, so a new ofsteam always replaces the previous one
	cout << "Open output file " << name << ". Writing started...\n";
	return;
}
//-----------------------------------------------------------------------------
void streams_switch()
{
	Token_stream ts(cin);
	Token t;
	while (true)
	try
	{
		activate_prompt();
		helper(ts, cout);	// skips all "print/spaces" and handle all "help"

		t = ts.get();

		switch (t.kind)
		{
		case to_file:
			set_ofstream(cin);
			break;
		case from_file:
			disable_prompt();
			read_file(ts);
	//		close_ofstream();
			break;
		case Exit:
			return;
		case stop_file: // ifstream closes on its return
			close_ofstream();
			break;
		default:
			ostream& os = fetch_ostream();
			ts.putback(t);
			calculate(ts, os);
			break;
		}
	}
	catch (exception& e) {
		cerr << "Error: " << e.what() << '.' << endl;
		close_ofstream();
		clean_up_mess(ts);
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------





//---------------------------------------------------------------------
double assignment(Token_stream& ts)
// assume we've seen assignment key (let)
// handle name '=' expression();
// assign value to an existing Variable. 
{
	Token t = ts.get();
	
	if (t.kind != name) {
		ts.putback(t); error("name expected in assignment");
	}

	string varname = t.name;
	if (!st.is_declared(varname))	error(varname, " not declared");
	if (st.is_constant_value(varname)) error("assignment to the constant ", varname); 

	Token t2 = ts.get();
	if (t2.kind != '=') { ts.putback(t2); error("= missing in assignment to ", varname); }

	double d = expression(ts);

	return st.set(varname, d);
}
//---------------------------------------------------------------------
double declaration(Token_stream& ts)
// assume: we've seen declaration key
// handle: name '=' expression();
// create: Variable with a name and a value.
{
	bool cnstant;
	Token t = ts.get();

		if (t.kind == constant) {
			t = ts.get();
			cnstant = true;
		} else cnstant = false;

		if (t.kind != name) {
			ts.putback(t);
			error("name expected in declaration");
		}

	string var = t.name;
		if (st.is_declared(var)) 	error(var, " already declared");
		
	
	Token t2 = ts.get();
		if (t2.kind != '=') {
			ts.putback(t2);
			error("= missing in declaration of ", var);
		}
	
	double d = expression(ts);
		if (cnstant) return st.declare_constant(var, d);
		return st.declare(var, d);
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
	
	intro(); 
	//------===========================
	
	st.declare_constant("pi", 3.1415926535);
	st.declare_constant("e", 2.7182818284);
	
	//st.fill_table();
	
	//------===========================
	
	streams_switch();

	keep_window_open();  // cope with windows console mode
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << '.' << endl;
	keep_window_open("~1");
	return 1;
}
catch (...) {
	cerr << "unexcpected failure\n";
	keep_window_open("~2");
	return 3;
}
//---------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------
void activate_prompt()
{
	Token_stream ts{ cin };
	ts.putback(Token());
	prompt_print(ts) = prompt;
}
//--------------------------------------------------------------------
void disable_prompt()
{
	Token_stream ts{ cin };
	ts.putback(Token());
	prompt_print(ts) = string{};
}
//--------------------------------------------------------------------
// prints the prompt by default if !full buffer
string& prompt_print(Token_stream& ts)
{
	static string p{ prompt };
	if (!ts.full_buffer() && !ts.full_istream_buffer()) cout << p; // always to the console 
	return p;
};
//--------------------------------------------------------------------
// skips all "print/spaces" and handle all "help" print 'prompt'
void helper(Token_stream& ts, ostream& os)
{
	Token t{ print };							//also skips spaces
	while (t.kind == print || t.kind == help) {	//first skip all "print" and handle all "help"	
		if (t.kind == help)
		{
			help_list(ts, os);
			intro();
		}
		prompt_print(ts);
		t = ts.get();
	}
	ts.putback(t);
	return;
}
//---------------------------------------------------------------------
string line(int length) 
{
	string s;
	for (int i = 0; i < length; ++i) s += char(-60);
	return s;
}
//---------------------------------------------------------------------
void help_items(ostream& os)
{
	os		<< " \n= What would you like to know of = \n"
			<< " 1. Variables and constants. \n"
			<< " 2. Operators. \n"
			<< " 3. Print all Variables. \n"
			<< " 4. Print all Constants. \n"
			<< " 5. Reading from and writing to files. \n"
			<< " 6. Numeral conversion system. \n"
			<< " r. Return. \n" 
			<< " -  Use any other simbol to print these items again." 
			<< endl;
}
//---------------------------------------------------------------------
void help_list(Token_stream& ts, ostream& os)
{
	help_items(os);
	char ch{ print };
	while (true) 
	{
		prompt_print(ts);
		ts.input_stream().get(ch);
		while (ch == print) {
			prompt_print(ts);
			ts.input_stream().get(ch);
		}

		switch (ch) 
		{
		case '1':
			os << '\n';
			os << line(57) << '\n';
			os		<< "  Variables and Constants info:\n"
					<< " - Declare new variables with '#': #n=5-2^2\n"
					<< " - Use 'const' to declare (#) constants: #const n=2+2\n"
					<< " - Change a variable with 'let': let n=2+2\n"
					<< " - Use them in expressions by typing their names: (a+b)*a\n"
					<< " - Allowed characters: A-z 0-9 _ . Names begin with A-z\n";
			os << line(57) << endl;
			break;
		case '2':
			os << '\n';
			os << line(40) << '\n';
			os		<< " Available orepators: \n"
					<< "\t () \n"
					<< "\t + " << " - \n"
					<< "\t / " << " * \n"
					<< "\t %       :remainder \n"
					<< "\t \"       :square root: \"n or \"n\"n that is \"(n * \"n) or \"\"n that is \"(\"n) "  << '\n'
					<< "\t !       :factorial: n! or n!!! that is ((n!)!)! '\n'"
					<< "\t ^       :to the power of: n^a or n^a^b that is n^(a^b) '\n'";
			os << line(40) << endl;
			break;
		case '3':
		{
			os << '\n';
			os << line(15) << '\n';
			os << " Variables:" << '\n';
			for (Simbol s : st.table) {
				if (!s.constant) cout << s.name << " = " << s.value << '\n';
			}
			os << line(15) << endl;
		}
		break;
		case '4':
		{
			os << '\n';
			os << line(15) << '\n';
			os << "  Constants:" << '\n';
			for (Simbol s : st.table) {
				if (s.constant) cout << s.name << " = " << s.value << '\n';
			}
			os << line(15) << endl;
		}
		break;
		case '5':
			os << '\n';
			os << line(78) << '\n';
			os		<< "  About Files:\n"
					<< "- 'from' to open a read file. E.g. from expressions.txt.\n"
					<< "- 'to' to open a write file. E.g. to output.txt.\n"
					//<< "- if using only 'to output.txt', all console results will be writed to a file.\n"
					<< "- 'stopfile' will stop readings and writing for all current files,\n"
					<< "  or you can also just use 'to' to start writing to another file.\n"
					<< "Hint: You can build a chain of files for reading:\n"
					<< "  e.g. use several \'from file1.txt to file2.txt\' within a single file,\n"
					<< "  or end a file with the 'from file3.txt' command to continue with the next one.\n"
				;
			os << line(78) << endl;
			break;
		case '6':
			os << '\n';
			os << line(50) << '\n';
			os		<< "Numeral system info:\n"
					<< "   bin   :introduses binary numeral. " << '\n'
					<< "   hex   :introduses hexadecimal numeral. " << '\n'
					<< "   oct   :introduses octal numeral. " << '\n'
					<< "   rom   :introduses roman numeral. " << '\n'
					<< "- Default in/output goes as decimal.\n"
					<< "- To convert use \"convto\". e.g. hex 8f convto bin," << '\n'
					<< "Hint: You can also use \"convto\" with expressions.\n"
					<< "	e.g. hex 8f + 15 * bin 00101 convto bin." << '\n'
					<< "- Only works with rational numbers.\n"
					<< "- Negative numerals representation: F at the left side of numeral for hex, \n"
					<< "	1 at the left side of numeral for binary,\n"
					<< "	7 at the left side of numeral for octal. (For more than 1 simbiols)\n"
					<< "- Positive numerals representation: 0 at the left side of numeral for hex, binary or octal. (For more than 1 simbiols)\n"
				;
			os << line(50) << endl;
			break;
		case 'r':
			ts.ignore(print);
			return;
		default:
			help_items(os);
			break;
		}
		ts.ignore(print);
	}
}

//---------------------------------------------------------------------


//Welcom to our simple calculator
 void intro() {
	cout	<< char(-77) << " Welcome to our simple calculator.\n"
			<< char(-77) << " Type expressions using floating-point number, end it with 'Enter' key.\n"
			<< char(-77) << ' ' << help_key << " - for help.\n"
			<< char(-77) << ' ' << exit_key << " - for quit.\n"
		;
 }
