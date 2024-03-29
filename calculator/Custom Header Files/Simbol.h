#pragma once
#include "std_lib_facilities.h"


//-----------------------------------------------------------------------------
// represents a named value
class Simbol {
public:
	string name;
	double value;
	bool constant;
	Simbol()
		: name(""), value(0), constant(0) {}
	Simbol(string n, double v, bool t)
		: name(n), value(v), constant(1) {}
	Simbol(string n, double v)
		: name(n), value(v), constant(0) {}
};

//---------------------------------------------------------------------

//vector of variables and constants
class Simbol_table {
public:
	vector<Simbol> table; //variables and constants
	double get(string);
	//void fill_table();
	bool is_constant_value(string);
	bool is_declared(string);
	double set(string, double);

	double declare_constant(const string&, const double&);
	double declare(const string&, const double&);
};



//ostream& operator<<(ostream& os, const Simbol& s);
//ifstream& operator>>(ifstream& is, Simbol& s);

