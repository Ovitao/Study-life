#include "Quering_Orders_Window.h"


//-----------------------------------------------------------------------------------------------------------------


void Querying_Orders_Window::calculate_total_price()
{
	if (current_list.size())
	{
		total_price = accumulate_orders(current_list, 0.0);
		redraw();
	}
}
/*
Query_Orders_Window::~Query_Orders_Window()
{
	for (auto a : cmps)
	{
		delete a.first;
		delete a.second;
	}
}*/
bool Querying_Orders_Window::is_descending()
{
	return  reference_to<Fl_Button>(sort_buttons.child(1)).value();
}
template<class Sort_Pattern>
void Querying_Orders_Window::sort(Sort_Pattern p)
{
	std::sort(current_list.begin(), current_list.end(), p
		//desc? *cmps[num].second :*cmps[num].first
	);
	redraw();
}
void Querying_Orders_Window::make_filter()
{
	filter.clear();
	for (size_t i = 0; i < filter_values.number_of_inputs(); i++)
	{
		string val = filter_values.input(i).value();
		if (val.size()) {
			filter.add(&filter_predicates[i]);
			filter_predicates[i].set_value(val);
		}
	}
}
void Querying_Orders_Window::make_list()
{
	if (ifs.is_open())
	{
		ifs.clear();
		ifs.seekg(0);
		total_price = -1;
		current_list = {};
		push_back_if_until(ifs, current_list, filter);
		//cout << ifs.fail() << ' ' << ifs.eof() << " 2f e \n ";
	//	ifs.close();
	}
	else
		ifs.clear(ios::failbit);
	redraw();
}

void Querying_Orders_Window::open_file_()
{
	ifs.close();
	ifs.open(file_name.value());
	/*
	if (ifs.is_open())
	{
		make_list();
		ifs.clear();
	}*/
	redraw();

}
void print_orders(const int x, int& y, int color, const int font, const int txt_size, const vector<Order>& current_list)
{
	fl_font(font, txt_size);
	fl_color(color);
	ostringstream os;
	for (const auto& ord : current_list)
	{
		os << ord;
		fl_draw(os.str().c_str(), x, y);
		os = {};
		y += txt_size;
	}

}


void Querying_Orders_Window::draw()
{
	if (visible())
	{
		Window::draw();
		if (!ifs.eof())
			if (ifs.fail())
			{
				fl_color(Color::red);
				fl_line_style(Line_style::solid, 3);
				fl_rect(file_name.x(), file_name.y(), file_name.w(), file_name.h());
				fl_draw("Something went wrong.", open_file.x() + open_file.w() + 5, open_file.y() + open_file.h() / 1.5);
			}
			else
				if (ifs.is_open())
					fl_draw("File is open.", open_file.x() + open_file.w() + 5, open_file.y() + open_file.h() / 1.5);
		int y = 20;
		int x = file_name.x() + file_name.w() + 50;
		
		print_orders(x, y, Color::black, Font::courier, 14, current_list);
	
		if (total_price != -1)
		{
			fl_color(FL_DARK_RED);
			fl_draw(string("Total Price: " + to_string(total_price)).c_str(), x, y);
		}
	}
}


Querying_Orders_Window::Querying_Orders_Window()
	:Window{ 1200,800,"Query Orders Window" },
	file_name{ 80,10,120,20,"File name" },
	open_file{ file_name.x(),file_name.y() + file_name.h() + 5,80,file_name.h() ,"Open file" },
	print{ file_name.x(),open_file.y() + file_name.h() + 5,open_file.w(),file_name.h() ,"Print" },
	_total_price{ print.x() + print.w() + 5,print.y(),open_file.w(),file_name.h() ,"Total price" },
	filter_values{ file_name.x(),print.y() + file_name.h() + 10,open_file.w(),file_name.h() ,"Filters",
		{
			new Fl_Input{0,0,0,0,"Name:"},
			new Fl_Input{0,0,0,0,"Address:"},
			new Fl_Input{0,0,0,0,"Data:"},
			new Fl_Input{0,0,0,0,"Purchases:"}
		}
		,
		{
			//	new Fl_Button{0,0,0,0,"Make List"}	
			}
},
filter_predicates
{
	new name_eq{""},
	new is_address_contains{""},
	new is_data_contains{""},
	new is_purchases_contain{""},
},
sort_buttons{ filter_values.x() + filter_values.w() + 5,filter_values.y(),filter_values.w(),file_name.h() ,"Sorting",
	{
		new Fl_Button{0,0,0,0,"Descending"},
		new Fl_Button{0,0,0,0,"Customer"},
		new Fl_Button{0,0,0,0,"Address"},
		new Fl_Button{0,0,0,0,"Data"},
		new Fl_Button{0,0,0,0,"Order Price"}
	}
},/*
cmps{
	pair< Comparison<less<>, Order>*, Comparison<greater<>, Order>* > {new by_name<less<>>{},new by_name<greater<>>{}},
	pair< Comparison<less<>, Order>*, Comparison<greater<>, Order>* > {new by_address<less<>>{},new by_address<greater<>>{}},
	pair< Comparison<less<>, Order>*, Comparison<greater<>, Order>* > {new by_data<less<>>{},new by_data<greater<>>{}},
	pair< Comparison<less<>, Order>*, Comparison<greater<>, Order>* > {new by_purchases_price<less<>>{},new by_purchases_price<greater<>>{}}

	}
,*/
exit{ x_max() - open_file.w() - 10,file_name.y(),open_file.w(), open_file.h(),"Exit" }
{
	//-----------------------sort
	sort_buttons.child(2)->callback(
		[](Fl_Widget*, void* v) {
			Querying_Orders_Window& qqw = reference_to<Querying_Orders_Window>(v);
			if (qqw.is_descending())
				qqw.sort(by_name<greater<>>());
			else
				qqw.sort(by_name<less<>>());
		}, this);
	sort_buttons.child(3)->callback(
		[](Fl_Widget*, void* v) {
			Querying_Orders_Window& qqw = reference_to<Querying_Orders_Window>(v);
			if (qqw.is_descending())
				qqw.sort(by_address<greater<>>());
			else
				qqw.sort(by_address<less<>>());
		}, this);
	sort_buttons.child(4)->callback(
		[](Fl_Widget*, void* v) {
			Querying_Orders_Window& qqw = reference_to<Querying_Orders_Window>(v);
			if (qqw.is_descending())
				qqw.sort(by_data<greater<>>());
			else
				qqw.sort(by_data<less<>>());
		}, this);
	sort_buttons.child(5)->callback(
		[](Fl_Widget*, void* v) {
			Querying_Orders_Window& qqw = reference_to<Querying_Orders_Window>(v);
			if (qqw.is_descending())
				qqw.sort(by_purchases_price<greater<>>());
			else
				qqw.sort(by_purchases_price<less<>>());
		}, this);
	sort_buttons.child(1)->callback(
		[](Fl_Widget* wd, void*) {
			Fl_Button& b = reference_to<Fl_Button>(wd);
			b.value(bool(b.value()) - 1); // switch
		}, this);
	add(sort_buttons);
	//-----------------------sort
	add(filter_values);
	add(_total_price);
	_total_price.callback(
		[](Fl_Widget*, void* v) {
			reference_to<Querying_Orders_Window>(v).calculate_total_price();

		}, this);
	/*
	Fl_Widget& make_list = filters.button(0);
	make_list.size(filters.w() - 20, file_name.h());
	make_list.callback([](Fl_Widget*, void* v) {
		hide_except_first(reference_to<Fl_Group>(v));
		reference_to<Query_Orders_Window>(v).make_filter();
		}, this);*/
	add(file_name);
	add(print);
	add(open_file);
	add(exit);

	print.callback([](Fl_Widget*, void* v) {
		Querying_Orders_Window& q = reference_to<Querying_Orders_Window>(v);
		q.make_filter();
		q.make_list();
		}, this);
	open_file.callback([](Fl_Widget*, void* v) { reference_to<Querying_Orders_Window>(v).open_file_(); }, this);
	exit.callback([](Fl_Widget*, void* v) {	reference_to<Querying_Orders_Window>(v).hide();	}, this);

	end();
}



//-----------------------------------------------------------------------------------------------------------------
void Fl_Menu_Group::add(Fl_Widget* wg)
{
	wg->hide();
	wg->resize(main_button.x(), child(children() - 1)->y() + main_button.h() + 5, w(), main_button.h());
	h((wg->y() - y()) + wg->h());
	Fl_Group::add(wg);
};
void Fl_Menu_Group::add(Fl_Widget& wg)
{
	wg.hide();
	wg.resize(main_button.x(), child(children() - 1)->y() + main_button.h() + 5, w(), main_button.h());
	//	wg.resize(main_button.x(), y() + h() + 5, w(), main_button.h());
	h((wg.y() - y()) + wg.h());
	Fl_Group::add(wg);
};
void hide_except_first(Fl_Group& mg)
{
	for (size_t i = 1; i < mg.children(); i++)
		mg.child(i)->hide();
}
void show_except_first(Fl_Group& mg)
{
	for (size_t i = 1; i < mg.children(); i++)
		mg.child(i)->show();
}
void switch_apperance_but_first(Fl_Group& mg)
{
	if (mg.children() > 1)
		if (mg.child(1)->visible()) hide_except_first(mg);
		else
			show_except_first(mg);
}
//-----------------------------------------------------------------------------------------------------------------



int Fl_Input_Menu_Group::number_of_inputs() const
{
	return inputs.size();
}
int Fl_Input_Menu_Group::number_of_buttons() const
{
	return buttons.size();
}
Fl_Button& Fl_Input_Menu_Group::button(int n)
{
	return buttons[n];
}
Fl_Input& Fl_Input_Menu_Group::input(int n)
{
	return inputs[n];
}

void Fl_Input_Menu_Group::add(Fl_Button* wg)
{
	Fl_Menu_Group::add(wg);
	buttons.push_back(wg);
};
void Fl_Input_Menu_Group::add(Fl_Input* wg)
{
	Fl_Menu_Group::add(wg);
	inputs.push_back(wg);
};

//-------------------------------------------------------------------------------------
