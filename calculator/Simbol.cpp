

#include  "Costom Header Files/Simbol.h"
#include "Costom Header Files/Other.h"
#include  "constants.cpp"

















//---------------------------------------------------------------------
// to get already defined variable or constant
double Simbol_table::get(string s)
{
	for (Simbol v : table)
		if (v.name == s) return v.value;
	error("Simbol get: undefined name ", s);
}
//---------------------------------------------------------------------
bool Simbol_table::is_constant_value(string s) {
	for (Simbol v : table)
		if (v.name == s && v.constant == true) return true;
	return false;
}
//---------------------------------------------------------------------
// to assign value to an existing variable 
double Simbol_table::set(string s, double d)
{
	if (is_constant_value(s)) error("Simbol set: assign to the constant ", s);

	for (Simbol& var : table)
		if (var.name == s) {
			var.value = d;
			return d;
		}
	error("Simbol set: undefined name ", s);
}
//---------------------------------------------------------------------
// is there already a simbol with the name declared
bool Simbol_table::is_declared(string s)
{
	for (int i = 0; i < table.size(); ++i)
		if (table[i].name == s) return true;
	return false;
}
//---------------------------------------------------------------------
//create a Constant with the 'name' and the 'value'
double Simbol_table::declare(string s, double d, bool c)
{
	
	if (is_declared(s)) error("Name already declared ", s);

	Simbol newsimbol{ s, d, bool(constant )};
	table.push_back(newsimbol);

	ofstream ofs{ "literal_numbers.txt" };
	if (!(ofs << newsimbol << '\n') && !ofs.eof())
			error("Can't open the literal_numbers.txt file for writing declared constant.\n");
	return d;
}

//---------------------------------------------------------------------
//create a Variable
double Simbol_table::declare(string s, double d)
{
	if (is_declared(s)) error("Name already declared ", s);
	Simbol newsimbol{ s, d };
	table.push_back(newsimbol);

	ofstream ofs{ "literal_numbers.txt" };
	if (!(ofs << newsimbol << '\n') && !ofs.eof())
		error("Can't write to literal_numbers.txt.\n");
	return d;
}
void Simbol_table::fill_table()
{
	ifstream ifs{ "literal_numbers.txt" };
	for (Simbol s; ifs >> s; table.push_back(s));
	if(!ifs && !ifs.eof()) error("Can't read literal_numbers.txt.\n");
	//not yet return ifs
}
//starts first in the program, if there is no file it will create the new
void create_list()
{
	static fstream fs{ "literal_numbers.txt" };
	if (!fs) static fstream fs("literal_numbers.txt");

};
ostream& operator<<(ostream& os, const Simbol &s) 
//name=val[is_const]
{
	char c;
	if (s.constant) c = 'c'; else c = 'n';
	os << s.name << '=' << s.value << '[' << c << ']';
	return os;
}
istream& operator>>(istream& is, Simbol& s) 
//name=val[is_const]
{
	string name, v;
	char c;

	//reading the Name
	if (is >> c && is_alpha(c)) { // the name starts with a letter
		name += c;
		while (is.get(c) && (is_alpha(c) || c == '_' || is_digit(c))) name += c;
	}
	if (c != '=') {
		is.unget();
		is.clear(ios_base::failbit);
		return is;
	}

	//reading value
	while (is.get(c) && is_digit(c) || c == '.') v += c;
	double val{ string_to_double(v) };
	
	// constant

	bool cnst{ is.get() == 'c' ? true : false };
	
	if (c != '[' && is.get() != ']') {
		is.unget();
		is.clear(ios_base::failbit);
	}
	s = { name,val,cnst };
	return is;
}
//---------------------------------------------------------------------