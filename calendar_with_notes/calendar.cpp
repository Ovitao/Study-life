#include "calendar.h"

//-------------------------------------------------------------------------------------------------------------------

Day_of_the_week& operator++(Day_of_the_week& dotw)
{
	if (dotw == Day_of_the_week::saturday)dotw = Day_of_the_week::sunday;
	else dotw = Day_of_the_week(int(dotw) + 1);
	return dotw;
}
//-------------------------------------------------------------------------------------------------------------------
Day::Day(int x, int y, int w, int h, string name_s, Month* m)
	:Named_Group{ name_s },
	write_or_read{ new Fl_Group{ 0, 0, element_size * window_size_multiplier, element_size * window_size_multiplier } },
	write{ new Fl_Group{0, 0, element_size * window_size_multiplier, element_size * window_size_multiplier } },
	read{ new Fl_Group{0, 0, element_size * window_size_multiplier, element_size * window_size_multiplier } },
	m{m}
{
	remove(child(0));

	add(new Fl_Button{ x, y, w, h, name.c_str() });
	child(0)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->write_or_read_in(); }, this);
	//-------------------------
	write_or_read->remove(write_or_read->child(0));

	add(write_or_read);
	write_or_read->hide();
	write_or_read->add(new Fl_Button{ 0, 0, element_size, element_size, "<" });
	write_or_read->child(0)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->write_or_read_back(); }, this);
	write_or_read->add(new Fl_Button{ 0, element_size, window_size_primary * (window_size_multiplier / 2), window_size_primary * (window_size_multiplier - 2), "Write Note." });
	write_or_read->child(1)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->write_in(); }, this);
	write_or_read->add(new Fl_Button{ window_size_primary * (window_size_multiplier / 2), element_size, window_size_primary * (window_size_multiplier / 2), window_size_primary * (window_size_multiplier - 2), "Read Note." });
	write_or_read->child(2)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->read_in(); }, this);
	//-------------------------

	//-------------------------

	write->hide();
	write->remove(write->child(0));

	write->add(new Fl_Button{ 0, 0, element_size * 3, element_size, "Save & <" });
	write->child(0)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->write_save_and_back(); }, this);
	write->add(new Fl_Input{ 0, element_size, window_size_primary * 8, window_size_primary * 8 });
	add(write);
	//-------------------------

	//-------------------------
	read->hide();
	read->add(new Fl_Button{ 0, 0, element_size, element_size, "<" });
	read->child(0)->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Day*>(iw)->read_back(); }, this);
	read->add(new Fl_Output{ 0, element_size, window_size_primary * 8, window_size_primary * 8 });
	add(read);
	//-------------------------
}
void Day::write_or_read_in()
{
	m->hide_primary_day_buttons();
	write_or_read->show();
	Fl::redraw();
}
void Day::write_or_read_back()
{
	write_or_read->hide();
	m->show_primary_day_buttons();
	Fl::redraw();
}
void Day::write_in()
{
	//cout << "write_in ";
	write_or_read->hide();
	write->show();
	Fl::redraw();
}
void Day::write_save_and_back()
{
	//cout << "write_save_and_back ";
	Fl_Input& v = *(reinterpret_cast<Fl_Input*>(write->child(1)));
	note = v.value();
	if (note.size()) primary_button()->color(Day::note_at_date);
	else primary_button()->color(49); //default grey color;
	write->hide();
	write_or_read->show();
	Fl::redraw();
}
void Day::read_back()
{
//	cout << "read_back ";
	read->hide();
	write_or_read->show();
	Fl::redraw();
}
void Day::read_in()
{
//	cout << "read_in ";
	Fl_Output& r = *(reinterpret_cast<Fl_Output*>(read->child(1)));
	write_or_read->hide();
	r.value(note.c_str());
	read->show();
	Fl::redraw();
}

//-------------------------------------------------------------------------------------------------------------------
const vector<string> months{ "Jan","Feb","Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug" ,"Sep" ,"Oct" ,"Nov" ,"Dec" };
const vector<int> days{31,28,31,30,31,30,31,31,30,31,30,31};

Month::Month(Day_of_the_week& dotw, int month_, Year* year, Calendar*cc)
	:Named_Group{ months[month_] }, month{ month_ }, y{ year }, c{ cc }
{
	int week{ 0 };
	bool leep_year = year->year == 2024 && month_ == 1;
	for (int n{ 0 }; n != (days[month]+leep_year); ) {
		while (n!= days[month] + leep_year)	{
			int x{ int(dotw) * element_size };
			int y{ (2*element_size )+ (element_size * week) };
			Day* db = new Day{ x,y,element_size,element_size,to_string(n+1),this};
			add(db);
			++dotw;
			++n;
			if (dotw == Day_of_the_week::sunday) break;
		}
		++week;
	}
	int x{ (month% 4) * element_size * 2 };
	int y{ element_size + ((month / 4) * element_size*2) };
	month_button = new Fl_Button{x ,y, element_size * 2, element_size * 2, name.c_str() };
	month_back_button = new Fl_Button{ 0, 0, element_size, element_size,"<" };
	add(month_button);
	add(month_back_button);
	month_button->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Month*>(iw)->month_in(); }, this);
	month_back_button->callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Month*>(iw)->month_back(); }, this);
	month_back();
}
void Month::month_in()
{
	show_primary_day_buttons();
	y->hide_month_buttons();
	c->hide_left_right();
	Fl::redraw();
}
void Month::month_back()
{
	hide_primary_day_buttons();
	y->show_month_buttons();
	c->show_left_right();
	Fl::redraw();
}
void Month::show_primary_day_buttons()
{
	int size_days = Month::children() - 2;
	for (size_t i = 0; i < size_days; i++)
	{	
		Day& r = *(reinterpret_cast<Day*>(child(i)));
		r.show_primary_button();
	}
	month_back_button->show();
	Fl::redraw();
}
void Month::hide_primary_day_buttons()
{
	int size_days = Month::children() - 2;
	for (size_t i = 0; i < size_days; i++)
	{
		Day& r = *(reinterpret_cast<Day*>(Month::child(i)));
		r.hide_primary_button();
	}
	month_back_button->hide();
	Fl::redraw();
}

void Month::hide_month_button()
{
	month_button->hide();
	//Fl::redraw();
}
void Month::show_month_button()
{
	month_button->show();
	//Fl::redraw();
}
void Month::draw()
{
	if (visible()) Named_Group::draw();
	if (month_back_button->visible())
	{
		constexpr int shift = element_size/3;
		constexpr int y = element_size * 1.8;
		fl_draw("S", shift, element_size *1.8);
		fl_draw("M", element_size+ shift, y);
		fl_draw("T", (element_size*2) + shift, y);
		fl_draw("W", (element_size * 3) + shift, y);
		fl_draw("T", (element_size * 4) + shift, y);
		fl_draw("F", (element_size * 5) + shift, y);
		fl_draw("S", (element_size * 6) + shift, y);
	}
}
//-------------------------------------------------------------------------------------------------------------------
Year::Year(Day_of_the_week& dotw, int year_, Calendar* cc)
	:Fl_Group{0,0,window_size_primary * window_size_multiplier ,window_size_primary * window_size_multiplier },
	year{ year_ },c{cc}
{
	for (int i = 0; i < months.size(); i++)
	{
		add(new Month{ dotw ,i,this, cc});
	}
}
void Year::draw()
{
	if (visible()) Fl_Group::draw();
	
	constexpr int shift = (window_size_primary * window_size_multiplier) / 2.4;
	constexpr int y = element_size;
	
	fl_draw(to_string(year).c_str(), shift, element_size-10);
}
void Year::hide_month_buttons()
{
	int size_months = children();
	for (size_t i = 0; i < size_months; i++)
	{
		Month& r = *(reinterpret_cast<Month*>(child(i)));
		r.hide_month_button();
	}
	Fl::redraw();
}
void Year::show_month_buttons()
{
	int size_months = children();
	for (size_t i = 0; i < size_months; i++)
	{
		Month& r = *(reinterpret_cast<Month*>(child(i)));
		r.show_month_button();
	}
	Fl::redraw();
}
//-------------------------------------------------------------------------------------------------------------------
void read_and_mark_notes(Calendar* c)
{
	ifstream ifs{ "notes.txt" };
	while (ifs)
		ifs >> *c;
}
void mark_current_date(Calendar* c)
{
	time_t now = time(0);
	tm bt{}; 
	localtime_s(&bt ,&now);
	int year{ 1900 + bt.tm_year };
	int month{ bt.tm_mon };
	int day{ bt.tm_mday -1 };
	Month& m = *(reinterpret_cast<Month*>(c->years[year]->child(month)));
	m.primary_button()->color(Day::current_date);

	Day& d = *(reinterpret_cast<Day*>(m.child(day)));
	d.primary_button()->color(Day::current_date);
}
Calendar::Calendar(int x, int y, const string& title)
	:Fl_Window{ x, y, window_size_primary * window_size_multiplier,window_size_primary * window_size_multiplier, title.c_str() },
	current_year{ min_year },
	to_left{ 0,0,element_size ,element_size ,"<<" },
	to_right{ (window_size_primary * window_size_multiplier) - element_size,0,element_size ,element_size ,">>" },
	Save_Exit {	(window_size_primary * window_size_multiplier) - element_size, (window_size_primary * window_size_multiplier) - element_size, element_size, element_size, "Exit"}
{
	for (int y = min_year; y <= max_year; y++){
		years.insert({ y, new Year{ day_of_the_week ,y,this } });
		add(years[y]);
		years[y]->hide();
	}
	years[min_year]->show();
	to_left.callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Calendar*>(iw)->to_left_(); }, this);
	to_right.callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Calendar*>(iw)->to_right_(); }, this);
	Save_Exit.callback([](Fl_Widget* wd, void* iw) { reinterpret_cast<Calendar*>(iw)->Save_and_Exit(); }, this);
	//----------------
	read_and_mark_notes(this);
	//----------------
	mark_current_date(this);
	//----------------
	fl_font(fl_font(), element_size);
	show();
}
void Calendar::Save_and_Exit()
{
	ofstream ifs{ "notes.txt" };
	for (int y = min_year; y <= max_year; y++) {
		for (size_t n= 0; n < years[y]->children(); n++){
			Month& m = *(reinterpret_cast<Month*>(years[y]->child(n)));
			
			for (size_t i = 0; i < m.children()-2; i++){
				Day& d = *(reinterpret_cast<Day*>(m.child(i)));
				if (d.note.size()) ifs << y << ' ' << m.month << ' ' << i << "{ " << d.note << " }\n";
			}
		}
	}
	hide();
}
istream& operator>>(istream& is, Calendar& c)
{
	int year, month, day;
	char ch;
	if (is >> year >> month >> day >> ch && ch == '{')
	{
 		Month& m = *(reinterpret_cast<Month*>(c.years[year]->child(month)));
		Day& d = *(reinterpret_cast<Day*>(m.child(day)));

		for (char chh;is.get(chh) && chh!= '}';)
			d.note += chh;
		d.primary_button()->color(Day::note_at_date);
		return is;
	}
	is.clear(ios::failbit);
	return is;
}
void Calendar::to_right_()
{
	if (max_year == current_year) return;
	years[current_year]->hide();
	++current_year;
	years[current_year]->show();
	Fl::redraw();
}
void Calendar::to_left_()
{
	if (min_year == current_year) return;
	years[current_year]->hide();
	--current_year;
	years[current_year]->show();
	Fl::redraw();
}
void Calendar::hide_left_right()
{
	to_left.hide();
	to_right.hide();
}
void Calendar::show_left_right()
{
	to_left.show();
	to_right.show();
}

Calendar::~Calendar()
{
	for (auto a : years)delete a.second;
}
//-------------------------------------------------------------------------------------------------------------------



int main()
try
{
	Calendar c{ window_size_primary , window_size_primary ,"Calendar" };
	Fl::run();
	cin.get();
}
catch (const std::exception& e)
{
	cerr << "Error: ";cout << e.what() << ".\n";
	cin.get();
}
catch (...)
{
	cerr << "Some error.\n";
	cin.get();
}
