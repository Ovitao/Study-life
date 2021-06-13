
#include "std_lib_facilities.h"

//implement system conversions

class Numerical_sysem {
public:
	double octal_to_decimal(string);
	double binary_to_decimal(string);
	double hexa_to_decimal(string);
	string decimal_to_binary(double);
	string decimal_to_hexa(double);
	string decimal_to_octal(double);
	string get_literal_from_decimal(double);
	double get_decimal_from_literal(char);
};