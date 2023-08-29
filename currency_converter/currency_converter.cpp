#include "../../../../Custom Header Files/Graphical_Headers/Simple_window.h"


/*
This converter implemented as 2 group of buttonns;
When a button with a known currency clicked corresponding buttons without known rated  go out.
It is enough to once define conversion rate in the file currency.txt to be able to convert to currency and vise versa;

*/


struct conversion_pattern
{
	conversion_pattern(string left_from, string right_to , double r)
		: orig{ left_from }, of_rate{ right_to }, rate{ r }{}
	conversion_pattern()
		: orig{ "" }, of_rate{ "" }, rate{ 0 }{}
	double inverse_rate() const
	{
		return 1.0 / rate;
	}
	string orig,of_rate;
	double rate;
};


bool operator==(const conversion_pattern& cp, const pair<string, string>& p)
{
	return (cp.orig == p.first && cp.of_rate == p.second) || (cp.orig == p.second && cp.of_rate == p.first);
}
bool operator==(const conversion_pattern& cp, string& p)
{
	return cp.orig == p || cp.of_rate == p;
}


bool is_there(const Fl_Group& buttons,const string &label, int& found_pos)
{
	for (found_pos = 0; found_pos < buttons.children(); found_pos++)
		if (buttons.child(found_pos)->label() == label) return true;
	return false;
 }
istream& operator>>(istream& is, conversion_pattern& cp)
{
	string orig, of_rate;
	double rate;

	for (char c; is >> c && c != '='; orig += c);
	for (char c; is >> c; of_rate += c)
		if (isdigit(c)) {
			is.unget();
			break;
		}
	is >> rate;

	//if (is) 
		cp = { orig ,of_rate ,rate };
	//	cout << cp.orig << ' ' << cp.of_rate << ' ' << cp.rate << '\n';
		return is;
}
class Currency_Converter : public Fl_Window
	// for file input form is USD = EUR 0.83   .can be read as 1 usd = 0.83 eur
	// you can only once define conversion. it will work vice versa
{
public:
	Currency_Converter(Point p, string currency_file);
	double get_rate(string from, string to) const;
	void draw() override;
private:
	vector<int> rates;
	vector<vector<int>> buttons_rate_relations;
	Vector_ref<conversion_pattern> patterns;
	Fl_Input inp_from;	
	Fl_Group menu_from;

	Fl_Button convert_button;
	
	Fl_Output out_result;
	Fl_Group menu_to;

	pair<Fl_Button*, Fl_Button*> current_selected;
	void convert();
	void set_current_selected_from(Fl_Button* );
	void set_current_selected_to(Fl_Button*);
	static void from_cb(Fl_Button* , Currency_Converter* );
	static void to_cb(Fl_Button* , Currency_Converter*);
	static void convert_cb(Fl_Button* , Currency_Converter* );
	void reactivate_buttons(Fl_Button* selected);
};
void Currency_Converter::from_cb(Fl_Button* cp, Currency_Converter* wp) 
{
	//cout << "from_cb " << cp << '\n';
	wp->set_current_selected_from(cp);
}
void Currency_Converter::to_cb(Fl_Button* cp, Currency_Converter* wp)
{
	//cout << "to_cb " << cp<<'\n';
	wp->set_current_selected_to(cp);
}
void Currency_Converter::convert_cb(Fl_Button*, Currency_Converter* wp)
{
	wp->convert();
}

void Currency_Converter::set_current_selected_from(Fl_Button* selected)
{
	//if(current_selected.first)
	current_selected.first->clear();
	current_selected.first = selected;
	selected->setonly();
	reactivate_buttons(selected);
}
void Currency_Converter::reactivate_buttons(Fl_Button* selected)
{
	//menu_to.deactivate();
	for (size_t i = 0; i < menu_to.children(); i++)
		menu_to.child(i)->deactivate();

	int n =	menu_from.find(reinterpret_cast<Fl_Widget*>(selected));
	//cout << "selected " << n << '\n';
	for (int p : buttons_rate_relations[n])
	{
		menu_to.child(p)->activate();
	//	cout << "menu_to.child(p)->activate(); " << p << '\n';
	}

	if(!current_selected.second->active()) current_selected.second->clear();
}
void Currency_Converter::set_current_selected_to(Fl_Button* selected)
{
	//if (current_selected.second)
	current_selected.second->clear();
	current_selected.second = selected;
	selected->setonly();
}
void print_vector(vector<vector<int>> vv)
{
	cout << "vector<vector<int>>------------\n";
	for(vector<int> v : vv){
		for (int n : v) cout << n << ' ';
		cout << '\n';
	}
	cout << "vector<vector<int>>------------\n";
}
double Currency_Converter::get_rate(string from, string to) const
{
	pair<string, string> p{ from, to };
	if (from == to) return 1;
	for (int n{ 0 };n<patterns.size();++n)
		if (patterns[n]== p)
			if (p.first == patterns[n].orig)
				return patterns[n].rate;
			else
				return patterns[n].inverse_rate();
	return 0;
}
void Currency_Converter::convert()
// 0 to out field if no input value or no rate
{
	istringstream is{ inp_from.value() };
	double value = 0;
	if (!is.str().empty())
		if (!(is >> value)) {out_result.value("Bad input value."); return;	}

	//convert
	double rate = 0;
	rate = get_rate(current_selected.first->label(), current_selected.second->label());
	if(!rate) out_result.value("Not selected.");
	 else out_result.value(to_string(value * rate).c_str());
}
Currency_Converter::Currency_Converter(Point p, string currency_file)
	:Fl_Window{ p.x,p.y,300,500,"Currency Converter" },
	inp_from{ 30,30,100,20,"" },
	menu_from{ inp_from.x(),60,80,y_max(),"" },
	convert_button{ inp_from.x()+ inp_from.w() + 10,inp_from.y() ,inp_from.h(),inp_from.h(),">" },
	out_result{ convert_button.x()+ convert_button.w() +10,inp_from.y(),inp_from.w(),inp_from.h(),"" },
	menu_to{ out_result.x(),menu_from.y(),menu_from.w(),menu_from.h(),"" },
	current_selected{ &convert_button,&convert_button } // unil first selection
{
	menu_from.remove(0);
	menu_from.remove(0);
	menu_from.remove(0);
	Fl_Window::add(inp_from);
	Fl_Window::add(out_result);
	convert_button.callback(reinterpret_cast<Fl_Callback*>(convert_cb), this);
	Fl_Window::add(convert_button);
	//------------------------
	ifstream ifs{ currency_file };
	if (!ifs) error("Can't open input file: ", currency_file);
	int y = menu_from.y();
	const int x1 = inp_from.x();
	const int x2 = out_result.x();
	const int width = menu_from.w();
	const int height = inp_from.h();
	bool is_orig = false;
	bool is_of_rate = false;

	while (ifs.good())
	{
		conversion_pattern current;
		if(!(ifs>>current)) 
			if (ifs.eof()) break; 
			else error("Bad input value.");
		patterns.push_back(new conversion_pattern{ current.orig, current.of_rate, current.rate });
		
		int orig_button_pos = 0; //if not found == number of buttons
		is_orig = is_there(menu_from, current.orig, orig_button_pos);
		if(!is_orig)
			// add one line of buttons
		{
			menu_from.add(new Fl_Button{ x1,y,width,height ,patterns[patterns.size()-1].orig.c_str() });
			buttons_rate_relations.push_back({ orig_button_pos }); // position of the same curency in the menu_to. USD == USD

			menu_to.add(new Fl_Button{ x2,y,width,height ,patterns[patterns.size() - 1].orig.c_str() });

			menu_from.child(menu_from.children() - 1)->callback(reinterpret_cast<Fl_Callback*>(from_cb), this);
			menu_to.child(menu_to.children() - 1)->callback(reinterpret_cast<Fl_Callback*>(to_cb), this);
			y += height;
		}

		int of_rate_button_pos=0; //if not found == number of buttons
		is_of_rate = is_there(menu_from, current.of_rate, of_rate_button_pos);
		if (!is_of_rate)
			// add one line of buttons
		{
			menu_from.add(new Fl_Button{ x1,y,width,height ,patterns[patterns.size() - 1].of_rate.c_str() });

			buttons_rate_relations.push_back({ of_rate_button_pos }); // position of the same curency in the menu_to. USD == USD
			
			// new button to the right
			// new button,another currency

			menu_to.add(new Fl_Button{ x2,y,width,height ,patterns[patterns.size() - 1].of_rate.c_str() });
			menu_to.child(menu_to.children() - 1)->callback(reinterpret_cast<Fl_Callback*>(to_cb), this);
			menu_from.child(menu_from.children() - 1)->callback(reinterpret_cast<Fl_Callback*>(from_cb), this);

			y += height;
		}
		
		buttons_rate_relations[orig_button_pos].push_back(of_rate_button_pos); // add position of found button to previous button pos
		buttons_rate_relations[of_rate_button_pos].push_back(orig_button_pos); // add position of previous button to found button pos
	}
	print_vector(buttons_rate_relations);
	Fl_Window::add(menu_from);
	Fl_Window::add(menu_to);

	show();
	//
	//
}
//Fl_Window{	p.x, p.y, out_result.x() + out_result.w() + inp_from.x(), menu_from.y() + menu_from.h(), "Currency Converter"
void Currency_Converter::draw()
{
	Fl_Window::draw();
	fl_draw("Currency value:", inp_from.x(), inp_from.y() - 5);
	fl_draw("Result:", out_result.x(), out_result.y() - 5);
}


int main()
try
{
	new	Currency_Converter { {100,100},"currency.txt" };
	

	return gui_main();
}
catch (const std::exception& e)
{
	cerr << "Error: " << e.what() << '.' << '\n';
	keep_window_open("1");
}
