#pragma once
#include "Order.h"


// Predicate, Comparison and Filter
//----------------------------------------------------------------------------------------
template<class Pred = less<>, class T1 = int, class T2 = T1> // expected less<> or greater<>
struct Comparison
{
	virtual bool operator()(const T1& o1, const T2& o2) const = 0;
};
template<class Pred = less<>> // excpected less<> or greater<>
struct by_name : public Comparison<Pred, Order>
{
	Pred pred{};
	bool operator()(const Order& o1, const Order& o2)const  override
	{
		return pred(tolower(o1.name), tolower(o2.name));
	}

};
template<class Pred = less<>>
struct by_address : public Comparison<Pred, Order>
{
	Pred pred{};
	bool operator()(const Order& o1, const Order& o2)const override
	{
		return pred(tolower(o1.address), tolower(o2.address));
	}
};
template<class Pred = less<>>
struct by_data : public Comparison<Pred, Order>
{
	Pred pred{};
	bool operator()(const Order& o1, const Order& o2)const override
	{
		return pred(tolower(o1.data), tolower(o2.data));
	}
};
template<class Pred = less<>>
struct by_purchases_price : public Comparison<Pred, Order>
{
	Pred pred{};
	bool operator()(const Order& o1, const Order& o2)const override
	{
		return pred(accumulate_purchases(o1, 0.0), accumulate_purchases(o2, 0.0));
	}
};
struct No_Sort
{
	bool operator()(const Order& o1, const Order& o2)
	{
		return false;
	}
};

//----------------------------------------------------------------------------------------

template<class Obj_type, class Value_type = string>
struct Predicate
{
	using value_type = Value_type;
	virtual void set_value(const Value_type&) = 0;
	virtual	bool operator()(const Obj_type&) const = 0;
};
struct name_eq : Predicate<Order>
{
	name_eq(string name)
		:name{ tolower(name) } {}
	string name;
	void set_value(const string& s) override
	{
		name = tolower(s);
	}
	bool operator()(const Order& o1) const override
	{
		return name == tolower(o1.name);
	}
};
//-------------------------------------------------------------------------------------

template <class In, class In2>
bool match(In f, In l, In2 f2, In2 l2)
{
	while (f != l && f2 != l2 && *f == *f2)
	{
		f++; *f2++;
	}
	if (f2 == l2) return true;
	return false;
}
template <class In>
bool is_found_txt(In f, In l, string s)
{
	if (s.size())
		while (f != l)
		{
			if (*f == s[0])
				if (match(f, l, s.begin(), s.end())) return true;
			++f;
		}
	return false;
}
//-------------------------------------------------------------------------------------
struct No_Pred
{
	bool operator()(const Order& o1)
	{
		return true;
	}
};
struct is_address_contains : Predicate<Order, string>
{
	is_address_contains(string country)
		:name{ tolower(country) } {}
	string name;
	void set_value(const string& s) override
	{
		name = tolower(s);
	}
	bool operator()(const Order& o1)const override
	{
		string address = tolower(o1.address);
		return is_found_txt(address.begin(), address.end(), name);
	}
};
struct is_data_contains : Predicate<Order, string>
{
	is_data_contains(string name)
		:txt{ tolower(name) } {}
	string txt;
	void set_value(const string& s) override
	{
		txt = tolower(s);
	}
	bool operator()(const Order& o1)const override
	{
		string data = tolower(o1.data);
		return is_found_txt(data.begin(), data.end(), txt);
	}
};


struct is_purchases_contain : Predicate<Order, string>
{
	is_purchases_contain(string txt)
		:txt{ tolower(txt) } {}
	string txt;
	void set_value(const string& s) override
	{
		txt = tolower(s);
	}
	bool operator()(const Order& o1)const override
	{
		for (const auto& a : o1.purchases)
		{
			string name = tolower(a.name);
			if (is_found_txt(name.begin(), name.end(), txt)) return true;
		}
		return false;
	}
};

//----------------------------------------------------------------------------------------
template<class Obj_type, class Value_type = string>
class Filter //by default true, No Predicate 
{
public:
	Filter() {};
	Filter(initializer_list<Obj_type> lst) : patterns{ lst } {}
	void clear() { patterns = {}; }
	void add(Predicate<Obj_type, Value_type>* val) { patterns.push_back(val); }

	bool operator() (const Obj_type& obj)const
	{
		for (size_t i = 0; i < patterns.size(); ++i)
			if (!patterns[i](obj)) return false;
		return true;
	}
private:
	Vector_ref<Predicate<Obj_type, Value_type>> patterns;
};
//----------------------------------------------------------------------------------------
