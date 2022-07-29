#pragma once
#include <numeric>
#include "../../../../Custom Header Files/Graphical_Headers/Gui.h"
using namespace Graph_lib;

class Purchase
{
public:
	string name; // product
	double unit_price;
	short count;
};
class Order
{
public:
	string name; //customer
	string address;
	string data;
	vector<Purchase> purchases;
};

istream& operator>>(istream& is, Purchase& p);
ostream& operator<<(ostream& os, const Purchase& p);
ostream& operator<<(ostream& os, const Order& o);
istream& operator>>(istream& is, Order& ord);





//---------------------------------------------------
string tolower(string s);
double accumulate_purchases(const Order& o, double init);


template<class Container>
double accumulate_orders(Container c, double init)
{
	return init + accumulate(c.begin(), c.end(), init, [](double& d, const Order& o) {	return 	accumulate_purchases(o, d); });
}

template<class Cont, class Pred>
void push_back_if_until(istream& istr, Cont& c, const Pred& pred)
{
	for (Value_type<Cont> o{}; istr >> o;)
		if (pred(o))
			c.push_back(o);
}