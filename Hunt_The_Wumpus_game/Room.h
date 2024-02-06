#pragma once
//#include "Hunt_The_Wumpus.h"
#include "facilities/Graphical_Headers/Graph.h";
//#include "facilities/std_lib_facilities.h";


using namespace Graph_lib;

static const Fl_Color grey64 = fl_rgb_color(64, 64, 64);
static const Fl_Color grey96 = fl_rgb_color(96, 96, 96);
static const Fl_Color grey128 = fl_rgb_color(128, 128, 128);


class Room : public Shape
{
public:
	Room() {
		set_fill_color(Color::black);
		tunnel(0) = nullptr;
		tunnel(1) = nullptr;
		tunnel(2) = nullptr;
	}
	int number{ 0 };
	void set_point(Point);
	Point point() const;
	bool wumpus_here{ false };
	bool bat_here{ false };
	bool has_pit{ false };
	void draw_lines() const;
	Room*& tunnel(unsigned); // returns refference to pointer so you can change the actual pointer and have easy and checked access;
private:
	Room* tunnels[3];
};



struct out_of_range_checker
{
	out_of_range_checker(unsigned int max)
		:max{ max }
	{	}
	void operator()(unsigned int n) const
	{
		if (n < max) {}
		else throw out_of_range("out of range: " + to_string(int(n)) + " [max]==" + to_string(max));
	}
private:
	unsigned int max;
};
static const out_of_range_checker room_checker{ 20 };


struct Message :public  Fl_GIF_Image
{
	Message(string name)
		:Fl_GIF_Image{ string{"messages/" + name + ".gif"}.c_str() }
	{	}

	virtual void room(Room*) { };

	virtual void draw(int x, int y)
	{
		Fl_GIF_Image::draw(x, y);
	}
};


struct general_message : public Message
{
	general_message()
		:Message{ "general" } {}

	void draw(int x, int y)override;
	void room(Room* r);
private:
	Room* room_;
	static constexpr char h_font = 36;
};


struct arrow_message : public Message
{
	arrow_message(string s)
		:Message{ s }
	{
		if (s[s.size() - 1] == '1') r_x = 390;
		else r_x = 430;
	}
	void draw(int x, int y) override;
	void room(Room* r) { if (!r) error("general_message; Room required;");	numbers.push_back(r->number + 1); };
private:
	int r_x;
	vector<int>numbers;
};

bool repeated_connection(Room* r);
char nulls(Room* r);
void connect(Room* r, Room* r2);
bool is_complete(Room* r);
bool is_connected(Room* r, Room* r2);
Room** null_ptr_of_tunnel(Room* r);
bool wumpus_nearby(Room* r);;
bool pit_nearby(Room* r);
bool bat_nearby(Room* r);

struct shoot_sequence
{
	pair<Room*&, bool&> r(int i) {
		if (i == 0) return { r1,rand1 };
		if (i == 1) return { r2,rand2 };
		if (i == 2) return { r3,rand3 };

	}

	Room* r1 = nullptr;
	Room* r2 = nullptr;
	Room* r3 = nullptr;
	bool rand1 = false;
	bool rand2 = false;
	bool rand3 = false;
};


struct err_num
{
	err_num(int n)
		:errn{ n } {}

	const int& what() const { return errn; }
private:
	int errn;
};


