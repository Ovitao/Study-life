/*
* 8/16/22
You can Print, Filter, Scroll(up/down) map of words.
When click "Print" Filter is created from visible filter fields;

*/


#include "words.cpp"
#include <numeric>
//#include <set>
//#include "../../../Custom Header Files/Graphical_Headers/Window.h"
#include "../../../Custom Header Files/Graphical_Headers/Gui.h"
using namespace Graph_lib;
using word = typename map<string, int>::value_type;
//using cb = void(Fl_Widget* ft, void* v);


//using stc = static_cast<Word_Queiry_Window*>;
int number_of_words(map<string, int> mp)
{
	return accumulate(mp.begin(), mp.end(), 0.0, [](double v, Value_type<map<string, int>>mp) {return v + mp.second; });
}
template<class s = int>
struct Comparison
{
	Comparison(const s& v) :val{v}{}
	s val;
	virtual void set_value(const s& v) {
		val = v;
	}
	virtual bool operator()(const word& w) const = 0;
};

class Word_filter
{
public:
	void clear();
	void add(Comparison<int>* );
	void add(Comparison<string>*);
	void add(Comparison<int>&);
	void add(Comparison<string>&);
	bool operator()(const word& w) const 
	{
		for (const auto& a : nt) if (!a(w)) return false;
		for (const auto& a : st) if (!a(w)) return false;
		return true;
	}
private:
	Vector_ref< Comparison<int>> nt;
	Vector_ref< Comparison<string>> st;
};
void Word_filter::clear()
{
	nt = {};
	st = {};
}

void Word_filter::add(Comparison<int>& c)
{
	nt.push_back(c);
}
void Word_filter::add(Comparison<string>& c)
{
	st.push_back(c);
}
void Word_filter::add(Comparison<int>* c)
{
	nt.push_back(c);
}
void Word_filter::add(Comparison<string>* c) 
{
	st.push_back(c);
}
template<class s>
struct number_of_letters: public Comparison<int>
{
	using Comparison::Comparison;
	s pred;
	bool operator()(const word& w) const override{
		return pred(w.first.size(), this->val);
	}
};

template<class s>
struct number_of_occurances : public Comparison<int>
{
	using Comparison::Comparison;
	s pred;
	bool operator()(const word& w) const override {
		return pred(w.second, this->val);
	}
};

ostream& operator<<(ostream& is, const Value_type<map<string, int>>& v)
{
	return is << v.first << " :" << v.second;
}
template<class In, class In2>
bool match(In first, In last, In2 first2, In2 last2) // does first sequence contains the second
{
	while (first != last && first2 != last2 && *first == *first2)
	{
		++first; ++first2;
	}
	return first2 == last2;
}
template<class In, class In2>
bool find_txt(In first, In last,const In2 first2, const In2 last2)
{
	while (first != last)
	{
		if (*first == *first2 && match(first, last, first2, last2)) return true;
		++first;
	}
	return false;
}
struct word_starts_with : public Comparison<string>
{
	using Comparison::Comparison;
	bool operator()(const word& w) const override {
		return match(w.first.begin(), w.first.end(), this->val.begin(), this->val.end());
	}
};

struct word_ends_in : public Comparison<string>
{
	using Comparison::Comparison;
	bool operator()(const word& w) const override {
		return match(w.first.rbegin(), w.first.rend(), this->val.rbegin(), this->val.rend());
	}
}; 
struct word_contains : public Comparison<string>
{
	using Comparison::Comparison;
	bool operator()(const word& w) const override {
		return find_txt(w.first.begin(), w.first.end(), this->val.begin(), this->val.end());
	}
};
map<string, int>::const_iterator max_occurances(const map<string, int>& l)
{
	auto max = l.begin();
	for (auto a = l.begin(); a!=l.end();++a )
		if (max->second < a->second) max = a;
	return max;
}

template<class Container>
class Scroll_List :public Fl_Group
{
public:
	Scroll_List(const Container& c, int x, int y, Fl_Window* win)
		:Fl_Group{ x,y,win->w() - x,win->h() - y }, cont{ c }, window{ win },
		up{ win->w() - 40, 0  ,40,40,"Up" },
		down{up.x(),win->h() - 40 ,up.w(),up.h() ,"Dwn" }, 
		first{ c.begin()}, begin{ c.begin() }, index{ 0 }
	{
		up.callback([](Fl_Widget*, void* v) {static_cast<Scroll_List*>(v)->up_(); }, this);
		down.callback([](Fl_Widget*, void* v) {static_cast<Scroll_List*>(v)->down_(); }, this);
		end();
	}
	int font{ Font::helvetica };
	int font_size{ 14 };
	bool print_second_part{ false };
	void up_(int n=3);
	void down_(int n=3);
	void set_first(unsigned);
	typename Container::const_iterator first_();
protected:
	void draw();
private:
	const Container& cont;
	Fl_Window* window;
	Fl_Button up;
	Fl_Button down;
	typename Container::const_iterator first;
	typename Container::const_iterator begin;
	unsigned index;
};
template<class C>
void Scroll_List<C>::set_first(unsigned s)
{
	if (s < cont.size())
	{
		index = s;
		begin = cont.begin();
		first = begin;
		advance(first, index);
	}
	else
		throw out_of_range("first out of range");
}

template<class C>
typename C::const_iterator Scroll_List<C>::first_()
{
	//typename C::const_iterator a = cont.begin();
	//advance(a, index);
	return first;
}

template<class C>
void Scroll_List<C>::up_(int n)
{
	for (char ch{ 0 }; ch < n && index && first!= cont.begin(); ++ch)
	{
		--first;
		--index;
	}
	window->redraw();
}
template<class C>
void Scroll_List<C>::down_(int n)
{
	int capacity = window->h() / font_size;
	int second_half = cont.size() - index;
	for (char ch{ 0 }; ch < n && capacity <= second_half--; ++ch)
	{
		++first;
		++index;
	}
	window->redraw();
}
template<class C>
void Scroll_List<C>::draw()
{
	Fl_Group::draw();
	if(cont.size())
	{ 
		fl_font(font, font_size);
		//fl_color(Color::black);

		if (begin != cont.begin())
		{
			index = 0; 
			begin = cont.begin();
			first = begin;
		}
		int  y{ Fl_Group::y() };
		auto a{ first };
		if(print_second_part)
		for ( ;a != cont.end() && y <= h(); ++a)
		{
			ostringstream os;
			os << *a;
			fl_draw(os.str().c_str(), x(), y);
			y += font_size;
		}
		else
		for (; a != cont.end() && y <= h(); ++a)
		{
			fl_draw(a->first.c_str(), x(), y);
			y += font_size;
		}
	}
}

class Word_Queiry_Window : public Window
{
public:
	using Window::Window;
	Word_Queiry_Window(const map<string, int>&);
protected:
	void draw();
private:
	Fl_Button list_b;
	map<string, int> current_list;
	Scroll_List<map<string, int>> scroll_page;
	const map<string, int> all_words;

	Fl_Button less_b;
	Fl_Button greater_b;
	Fl_Button equal_b;

	Fl_Button num_letters_b;
	Fl_Button num_occurances_b;

	Fl_Button filter_b;
	Fl_Input starts_with_f;
	Fl_Input ends_with_f;
	Fl_Input contains_f;

	Fl_Button display_numbers;

	Vector_ref<Comparison<int>> cmps;
	Vector_ref<Comparison<string>> cmps_2;
	Fl_Input nubmer_f;
	Word_filter wf;
	char filt_num{ 0 };
	template<class Pred>
	void make_list(const Pred&);
	void create_filter_and_list();
	void num_show_hide(char ch =0);
	void switch_filter_b();
};

void Word_Queiry_Window::switch_filter_b()
{
	if (filter_b.visible())
	{
		filter_b.hide();
		starts_with_f.show();
		ends_with_f.show();
		contains_f.show();
	}
	else {
		filter_b.show();
		starts_with_f.hide();
		ends_with_f.hide();
		contains_f.hide();
	}
}

template<class In,class Cont,class Pred>
void map_insert_if(In f, In l,Cont& c, const Pred& pred)
{
	while (f != l) {
		if (pred(*f))
			c.insert(*f);
			//c.puch_back(*f);
		++f;
	}
}
template<class Pred>
void Word_Queiry_Window::make_list(const Pred& pred)
{
	current_list = {};
	map_insert_if(all_words.begin(), all_words.end(), current_list, pred);
}
void  Word_Queiry_Window::create_filter_and_list()
{
	wf.clear();
	if(nubmer_f.visible())
	{	
		istringstream ist{ nubmer_f.value() };
		int n{ 0 };
		ist >> n;
		cmps[filt_num].set_value(n);
		wf.add(cmps[filt_num]);
		num_show_hide();
	}
	//if (starts_with_f.visible())
	{
		string value = starts_with_f.value();
		if (value.size()){
			wf.add(cmps_2[0]);
			cmps_2[0].set_value(value);
		}
		value = ends_with_f.value();
		if (value.size()) {
			wf.add(cmps_2[1]);
			cmps_2[1].set_value(value);
		}
		value = contains_f.value();
		if (value.size()) {
			wf.add(cmps_2[2]);
			cmps_2[2].set_value(value);
		}
		//switch_filter_b();
	}
	//else		make_list([](const Value_type<map<string, int>>&) {return true; });
	make_list(wf);
	filt_num = 0;
	wf.clear();
	redraw();
}
void Word_Queiry_Window::num_show_hide(char ch)
{
	filt_num += ch;
	if(num_letters_b.visible())
	{
		num_letters_b.hide();
		num_occurances_b.hide();

		greater_b.show();
		less_b.show();
		equal_b.show();
	}
	else
	if (greater_b.visible())
	{
		greater_b.hide();
		less_b.hide();
		equal_b.hide();

		nubmer_f.show();
	}
	else 
	{
		nubmer_f.hide();
		num_letters_b.show();
		num_occurances_b.show();
	}
	redraw();
}
void Word_Queiry_Window::draw()
{
	if (visible())
	{
		Window::draw();
		if(num_letters_b.visible())
			fl_draw("Number of..", num_letters_b.x(), num_letters_b.y() - 2);
		if (nubmer_f.visible())
			fl_draw("Number is: ", num_letters_b.x(), num_letters_b.y() - 2);
		//if (starts_with_f.visible())
		{
			fl_draw("Starts with: ", starts_with_f.x(), starts_with_f.y() - 2);
			fl_draw("Ends with: ", ends_with_f.x(), ends_with_f.y() - 2);
			fl_draw("Contains: ", contains_f.x(), contains_f.y() - 2);
		}
	}
}

Word_Queiry_Window::Word_Queiry_Window(const map<string, int>& mp)
	:Window({ 200,100 }, 400, 800, "Word_Queiry_Window"),
	list_b{ 10,10,90,20,"Print" },
	scroll_page{ current_list,120,10,this },
	all_words{ mp }, 
	
	less_b{ list_b.x(),list_b.y() + list_b.h() + 20,list_b.w(),list_b.h(),"less than.." },
	greater_b{ list_b.x(),less_b.y() + list_b.h() + 5,list_b.w(),list_b.h(),"greater than.." },
	equal_b{ list_b.x(),greater_b.y() + list_b.h() + 5,list_b.w(),list_b.h(),"equal to.." },
	num_letters_b{ less_b.x(),less_b.y(),less_b.w(),less_b.h(),"letters" },
	num_occurances_b{ greater_b.x(),greater_b.y(),greater_b.w(),greater_b.h() ,"occurances" },

	filter_b{ equal_b.x(), equal_b.y() + equal_b.h() + 15,equal_b.w(),equal_b.h(),"Filters"},
	starts_with_f{ filter_b.x(), filter_b.y(),filter_b.w(),filter_b.h(),"" },
	ends_with_f{ filter_b.x(), starts_with_f.y() + filter_b.h() + 15,filter_b.w(),filter_b.h(),"" },
	contains_f{ filter_b.x(), ends_with_f.y() + filter_b.h() + 15,filter_b.w(),filter_b.h(),"" },
	
	display_numbers{ list_b.x(),300,less_b.w(),less_b.h(), "Displ. occur." }, //200
	cmps{ 
		new number_of_letters<less<>>(0),
		new number_of_letters<greater<>>(0),
		new number_of_letters<equal_to<>>(0),
		new number_of_occurances<less<>>(0),
		new number_of_occurances<greater<>>(0),
		new number_of_occurances<equal_to<>>(0)
	},
	cmps_2{
		new word_starts_with{""},
		new word_ends_in{""},
		new word_contains{""}
	},
	nubmer_f{ less_b.x(),less_b.y(),less_b.w(),less_b.h(),"number is: " }
{
//	nubmer_f.align(FL_ALIGN_LEFT_TOP);
	add(display_numbers);	add(scroll_page); add(filter_b);
	add(list_b);	add(less_b);	add(equal_b); add(greater_b);
	add(num_occurances_b);	add(num_letters_b);	add(nubmer_f);	
	add(ends_with_f);	add(contains_f);	add(starts_with_f);
	switch_filter_b(); // wount use
	//starts_with_f.hide();	ends_with_f.hide();	contains_f.hide();
	greater_b.hide();
	less_b.hide();
	equal_b.hide();	
	nubmer_f.hide();
	filter_b.callback([](Fl_Widget*, void* v) {static_cast<Word_Queiry_Window*>(v)->switch_filter_b();	}, this);

	display_numbers.callback([](Fl_Widget* b, void* v) { 
		Fl_Button* bb = static_cast<Fl_Button*>(b);
		bb->value(bool(bb->value())-1);
		static_cast<Word_Queiry_Window*>(v)->scroll_page.print_second_part = bb->value();
		static_cast<Word_Queiry_Window*>(v)->redraw();
		}, this);
	list_b.callback([](Fl_Widget*, void* v) { static_cast<Word_Queiry_Window*>(v)->create_filter_and_list(); }, this);
	num_letters_b.callback([](Fl_Widget*, void* v) {static_cast<Word_Queiry_Window*>(v)->num_show_hide(); }, this);
	num_occurances_b.callback([](Fl_Widget*, void* v) {	static_cast<Word_Queiry_Window*>(v)->num_show_hide(3);}, this);
	
	less_b.callback([](Fl_Widget*, void* v) {static_cast<Word_Queiry_Window*>(v)->num_show_hide(0);	}, this);
	greater_b.callback([](Fl_Widget*, void* v) {static_cast<Word_Queiry_Window*>(v)->num_show_hide(1);	}, this);
	equal_b.callback([](Fl_Widget*, void* v) {static_cast<Word_Queiry_Window*>(v)->num_show_hide(2);	}, this);
}

/*
struct txt_comtains
{
	string txt;
	bool operator() const 
	{

	}
};


struct A
{
	string s;
	int n;
};

struct cmp
{
	bool operator()(const A& a1, const A& a2) const
	{
		return a1.s < a2.s;
	}
};

struct cmp_n
{
	string s;
	cmp_n(string s) :s{s}{}
	bool operator()(const A& a1) const
	{
		return a1.s ==s;
	}
};*/
/*
void f(Container& c)
{

}
*/
int main()
try
{
	//to_string(1);
//	map<A, int, cmp> mp{ { A{"c",1},4 },{ A{"a",3},5 },{ A{"b",2},6 } };

	//mp.find(cmp_n("c"));
/*	Vector_ref<int> v{ new int {1},new int {2}, new int {3} };



	for (int n : v)
		cout << n << '\n';
	keep_window_open("End.");
	*/
	ifstream ifs{ "paper.txt" };
	if (!ifs) throw exception("bad paper.txt");

	map<string, int> words = get_words(ifs);

	Word_Queiry_Window wqw{words};
	Fl::run();





	/*
	for (short ch; ; )
	{
		cout << "Words query program, select question\n";
		cout << "1. Number of words.\n";
		cout << "2. Lins all.\n";
		cout << "3. number of letters..\n";
		cin >> ch;
		switch (ch)
		{
		case 1:
			cout << "Number of words " << number_of_words(words);
			break;
		case 2:
			print_map_2(words, [](word) {return true; });
			break;
		case 3:
		{			cout << "Words query program, select question\n";
			cout << "1. less than..\n";
			cout << "2. greater than..\n";
			cout << "3. equal to..\n";
			cin >> ch;
			cout << "number is: ";
			int n;
			cin >> n;

			Comparison<int>* cmp = 0;
			switch (ch)
			{
			case 1:
				cmp = &number_of_letters<less<>>{ n };
				break;
			case 2:
				cmp = &number_of_letters<greater<>>{ n };
				break;
			default:
				cmp = &number_of_letters<equal_to<>>{ n };
				break;
			}
				print_map_2(words, *cmp);
			break;
		}
		default:
			keep_window_open("End.");
			break;
		}
	}

	
	*/

}
catch (const std::exception& e)
{
	cerr << e.what();
	keep_window_open("~");
}