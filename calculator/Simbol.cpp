

#include "Custom Header Files/Simbol.h"
#include "Custom Header Files/Other.h"
#include "constants.cpp"

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

	table.push_back(Simbol(s, d, constant));

	return d;
}

//---------------------------------------------------------------------
//create a Variable
double Simbol_table::declare(string s, double d)
{
	if (is_declared(s)) error("Name already declared ", s);

	table.push_back(Simbol(s, d));

	return d;
}
/*
//---------------------------------------------------------------------
//create a Constant with the 'name' and the 'value'
double Simbol_table::declare(string name, double value, bool c)
{
	if (is_declared(name)) error("Name already declared ", name);
	static int t{};
	
	Simbol newsimbol{ name, value, bool(constant )};
	table.push_back(newsimbol);

	ofstream ofs;
	ofs.open("literal_numbers.txt", ios_base::out);
	if (!(ofs << newsimbol << '\n') && !ofs.eof())
		error("Can't open the literal_numbers.txt file for writing declared constant.\n");
	return value;
}

//---------------------------------------------------------------------
//create a Variable
double Simbol_table::declare(string s, double d)
{
	if (is_declared(s)) error("Name already declared ", s);
	Simbol newsimbol{ s, d };
	table.push_back(newsimbol);

	ofstream ofs;
	ofs.open("literal_numbers.txt", ios_base::out);
	if (!(ofs << newsimbol << '\n') && !ofs.eof())
		error("Can't write to literal_numbers.txt.\n");
	return d;
}
//---------------------------------------------------------------------

void Simbol_table::fill_table()
{
	ifstream ifs;
	ifs.open("literal_numbers.txt", ios_base::in);
	if (!ifs) error("fill_table(): can't open literal_numbers.txt");

	for (Simbol s; ifs >> s; table.push_back(s));

	if(!ifs && !ifs.eof()) error("Can't read literal_numbers.txt.\n");
	//not yet return ifs
}
//---------------------------------------------------------------------

ostream& operator<<(ostream& os, const Simbol &s) 
//name val const
//name val not_const
{
	string cc;
	if (s.constant) cc = "const"; 
	else cc = "not_const";
	return os << s.name << ' ' << s.value << ' ' << cc;

}
//---------------------------------------------------------------------

//---------------------------------------------------------------------

ifstream& operator>>(ifstream& is, Simbol& s)
//name val const
//name val not_const
{
	string name, v;
	double val;

	if (!(is >> name >> val >> v)) error("bad Simbol reading");

		if (v=="const") s = { name, val, true };
	else 
		if (v == "not_const") s = { name,val };
	else error("bad Simbol const reading");
		return is;
}
//---------------------------------------------------------------------

/*name=val[is_const]
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
*/

//---------------------------------------------------------------------