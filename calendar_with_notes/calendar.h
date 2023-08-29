#pragma once


/*
* Calendar With Notes:
This calendar holds 2023-2025 years;
current date marked as red;
date with note marked as cyan
Exit with Exit button to save notes to the file "notes.txt"

*/
#include "fltk.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>  

using namespace std;
//--------------------------------------------------------------------------
constexpr int min_year{ 2023 };
constexpr int max_year{ 2025 };
constexpr int element_size{ 30 }; //also font_size
constexpr int window_size_multiplier{ 12 };
constexpr int window_size_primary{ 20 };
//--------------------------------------------------------------------------
class Named_Group :public Fl_Group { // fixes issue when name dissapiars after string holding the name goes out of scope;
public:
	string name;
	Named_Group(
		//int x, int y, int w, int h, 
		string name_s
	)
		:name{ name_s },
		Fl_Group{ 0, 0, element_size * window_size_multiplier,  element_size * window_size_multiplier,name.c_str() } {}
};
//--------------------------------------------------------------------------
enum class Day_of_the_week {
	sunday,
	monday,
	tuesday,
	wednesday,
	thursday,
	friday,
	saturday
};
Day_of_the_week& operator++(Day_of_the_week& dotw);
//--------------------------------------------------------------------------
class Calendar;
class Year :public Fl_Group
{
	Calendar* c;
protected:
	void draw();
public:
	const int year;
	void hide_month_buttons();
	void show_month_buttons();
	Year(Day_of_the_week& dotw, int year, Calendar* c);
};
//--------------------------------------------------------------------------
class Calendar :public Fl_Window
{
public:
	Calendar(int x, int y, const string& title);
	void hide_left_right();
	void show_left_right();
	~Calendar();
	map<int, Year*> years;
private:
	void to_left_();
	void to_right_();
	void Save_and_Exit();
	Day_of_the_week day_of_the_week{ Day_of_the_week::sunday };
	int current_year;
	Fl_Button to_left;
	Fl_Button to_right;
	Fl_Button Save_Exit;
};
istream& operator>>(istream& is, Calendar& c);
//--------------------------------------------------------------------------
class Month :public Named_Group
{
	Fl_Button* month_button;
	Fl_Button* month_back_button;
	Year* y;
	Calendar* c;
protected:
	void draw() override;
public:
	const int month;
	void month_in();
	void month_back();
	Fl_Button* primary_button() { return month_button; }
	void hide_month_button();
	void show_month_button();
	void hide_primary_day_buttons(); // also back button
	void show_primary_day_buttons();// also back button
	Month(Day_of_the_week& dotw, int month, Year* y, Calendar*);
};
//--------------------------------------------------------------------------
class Day :public Named_Group {
	Fl_Group* write_or_read;
	Fl_Group* write;
	Fl_Group* read;
	Month* m;
	void write_or_read_in();
	void write_or_read_back();
	void write_in();
	void read_in();
	void write_save_and_back();
	void read_back();
public:
	enum {
		note_at_date = FL_CYAN,
		current_date = FL_RED
	};
	Fl_Widget* primary_button() {return child(0); }
	void hide_primary_button() { child(0)->hide(); }
	void show_primary_button() { child(0)->show(); }
	string note;
	Day(int x, int y, int w, int h, string name_s, Month* mb);
};
//--------------------------------------------------------------------------


