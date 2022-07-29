//#include "../../../../Custom Header Files/std_lib_facilities.h"
//#include "../../../../Custom Header Files/Graphical_Headers/Window.h"
//#include "../../../../Custom Header Files/Graphical_Headers/Gui.h"
//#include "../../../../Custom Header Files/Graphical_Headers/Simple_window.h"
//#include <numeric>
#include "Order.h"



istream& operator>>(istream& is, Purchase& p)
// name unit_price count
{
	return is >> p.name >> p.unit_price >> p.count;
}

ostream& operator<<(ostream& os, const Purchase& p)
// name unit_price count
{
	return os << p.name << ' ' << p.unit_price << ' ' << p.count;
}
ostream& operator<<(ostream& os, const Order& o)
// name unit_price count
{
	os << o.name << " { " << o.address << " } " << " { " << o.data << " } " << " {";
	for (short n{ 0 }; n < o.purchases.size(); ++n)
	{
		os << ' ' << o.purchases[n];
		if (n != o.purchases.size() - 1)
			os << ',';
		//	else			break;
	}
	return os << " }";
}
void read_until(istream& is, string& s, const char& end)
{
	char ch;
	is >> ch;// cut spaces at the begining
	while (is && ch != end)
	{
		s += ch;
		if (isspace(unsigned char(ch)))
			is >> ch;
		else
			is.get(ch);
	}
	if (isspace(s.back()))
		s.pop_back(); // cut spaces at the end
}

istream& operator>>(istream& is, Order& ord)
// name {address} {data} {Purchase,Purchase}
{
	if (!is || is.eof()) return is;
	Order o;
	char ch;
	read_until(is, o.name, '{');
	read_until(is, o.address, '}');
	if (is >> ch && ch == '{')
		//	while (is.get(ch) && ch != '}') o.data += ch;
		read_until(is, o.data, '}');


	if (is >> ch && ch == '{')
		for (Purchase p; is >> p; )
		{
			o.purchases.push_back(p);
			is >> ch;
			if (ch != ',') break;
		}
	if (is && ch != '}')
		is.clear(ios::failbit);
	else
		ord = o;
	return is;
}

//---------------------------------------------------------
string tolower(string s)
{
	for (char& c : s) c = tolower(c);
	return s;
}


double accumulate_purchases(const Order& o, double init)
{
	return accumulate(o.purchases.begin(), o.purchases.end(), init, [](double& v, const Purchase& p) {	return 	v + (p.count * p.unit_price); });
}

