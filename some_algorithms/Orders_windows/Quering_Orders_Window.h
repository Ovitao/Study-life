#pragma once
#include "Order.h"
#include "function_objects.h"
/*
file reading state is indicated with a text message or red square around the text field.
in the window you can (sort and filer not case-sensitive):
open a file,
print orders,
sort (descending or ascending) by: name, total price of Purchases of single order, address or data;
filter by: customer name(equals), address(if_any),data(if_any),purchases names(if_any).

*/
//--------------------------------------------------------------------------------------
void switch_apperance_but_first(Fl_Group& mg);
//--------------------------------------------------------------------------------------

class Fl_Menu_Group : public Fl_Group
{
public:
	Fl_Menu_Group(int x, int y, int w, int h, const char* title, initializer_list<Fl_Widget*>lst)
		:Fl_Menu_Group{ x, y, w, h, title }
	{
		for (auto& obj : lst) add(obj);
	}

	Fl_Menu_Group(int x, int y, int w, int h, const char* title)
		:Fl_Group{ x, y, w, h },
		main_button{ x, y, w, h, title }
	{
		//Fl_Group::add(main_button);
		main_button.callback([](Fl_Widget*, void* gr) { switch_apperance_but_first(reference_to<Fl_Group>(gr)); }, this);
		main_button.color(FL_DARK1);
		end();
	}

	Fl_Button main_button;

	void add(Fl_Widget*);
	void add(Fl_Widget&);
};
//--------------------------------------------------------------------------------------
class Fl_Input_Menu_Group : public Fl_Menu_Group
{
public:
	Fl_Input_Menu_Group(int x, int y, int w, int h, const char* title, initializer_list<Fl_Input*>lst_1, initializer_list<Fl_Button*>lst_2)
		:Fl_Menu_Group{ x, y, w, h, title }
	{
		for (auto& obj : lst_1) add(obj);
		for (auto& obj : lst_2) add(obj);
	}

	void add(Fl_Input*);
	void add(Fl_Button*);
	int number_of_inputs() const;
	Fl_Input& input(int);
	int number_of_buttons() const;
	Fl_Button& button(int);
private:
	Vector_ref<Fl_Input> inputs;
	Vector_ref<Fl_Button> buttons;
};

//--------------------------------------------------------------------------------------


//void Fl_Menu_Group::show_except(){for (size_t i = 1; i < children(); i++)	child(i)->show();}
class Querying_Orders_Window : public Window
{
public:
	//	using Window::Window;

	Querying_Orders_Window();
	//	~Query_Orders_Window();
protected:
	void draw();
private:
	Fl_Input file_name;
	Fl_Button open_file;
	Fl_Button print;
	Fl_Button exit;
	//-------------- 
	ifstream ifs;
	Fl_Input_Menu_Group filter_values;

	Vector_ref<Predicate<Order, string>> filter_predicates;
	Filter<Order> filter;
	void open_file_();
	void make_list();
	void make_filter();
	vector<Order> current_list;
	//--------------
	Fl_Menu_Group sort_buttons;
	template<class Sort_Pattern>
	void sort(Sort_Pattern);
	bool is_descending();
	//vector<pair<Comparison<less<>, Order>*, Comparison<greater<>, Order>*>> cmps; unfortunately, it isn't possible to use base class with the standart sort() because it copies the arguments  
	//---------------
	Fl_Button _total_price;
	double total_price{ -1 };
	void calculate_total_price();
};

//--------------------------------------------------------------------------------------
