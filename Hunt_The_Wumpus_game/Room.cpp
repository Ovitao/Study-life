#include "Room.h"

void Room::draw_lines() const
{
	constexpr int radius{ 25 };

	fl_color(fill_color().as_int());
	fl_pie(point().x - radius, point().y - radius, radius * 2, radius * 2, 0, 360);

	fl_color(grey64);
	fl_line_style(Line_style::solid, 3);
	fl_arc(point().x - (radius + 2), point().y - (radius + 2), ((radius + 2) * 2), ((radius + 2) * 2), 0, 360);

	fl_color(grey128);
	fl_draw(to_string(number + 1).c_str(), point().x - 6, point().y + 5);
}
void Room::set_point(Point p)
{
	if (number_of_points()) Shape::set_point(0, p);
	else add(p);
}
Point Room::point() const
{
	return Shape::point(0);
}

Room** null_ptr_of_tunnel(Room* r)
// pointer to pointer, check on whether the room is complete before 
{
	for (char i = 0; i < 3; i++)
		if (!r->tunnel(i)) return &r->tunnel(i);
	return 0;
}

bool is_connected(Room* r, Room* r2)
{
	return !r || !r2 || (r2->tunnel(0) == r || r2->tunnel(1) == r || r2->tunnel(2) == r)
		&& (r->tunnel(0) == r2 || r->tunnel(1) == r2 || r->tunnel(2) == r2)
		;
}

bool is_complete(Room* r)
{
	return r->tunnel(0) && r->tunnel(1) && r->tunnel(2);
}

void connect(Room* r, Room* r2)
{
	if (is_complete(r) || is_complete(r2) || is_connected(r, r2)) return;
	*null_ptr_of_tunnel(r2) = r;
	*null_ptr_of_tunnel(r) = r2;
}
Room*& Room::tunnel(unsigned n)
{
		if(n<3) return tunnels[n];
		throw range_error("tunnel: out of range: " + to_string(int(n)));
}

char nulls(Room* r)
{
	char n = 0;
	n += !bool(r->tunnel(0));
	n += !bool(r->tunnel(1));
	n += !bool(r->tunnel(2));
	return n;
}
bool repeated_connection(Room* r)
{
	for (char i = 0; i < 3; i++)
	{
		if (r->tunnel(i) == r)  return true;

		for (char c = 0; c < 3; c++)
			if (i != c && r->tunnel(i) == r->tunnel(c))  return true;
	}
	return false;
}



bool bat_nearby(Room* r)
{
	for (char i = 0; i < 3; i++)
		if (r->tunnel(i)->bat_here)
			return true;
	return false;
}
bool pit_nearby(Room* r)
{
	for (char i = 0; i < 3; i++)
		if (r->tunnel(i)->has_pit)
			return true;
	return false;
}
bool wumpus_nearby(Room* r)
{
	for (char i = 0; i < 3; i++)
		if (r->tunnel(i)->wumpus_here)
			return true;
	return false;
}
void general_message::draw(int x, int y)
{
	Message::draw(x, y);
	static constexpr int r_n = 270, t_n = 440;

	Message{ to_string(room_->number + 1).c_str() }.draw(x + r_n, y);
	y += h_font;

	for (char i = 0; i < 3; i++)
		Message{ to_string(room_->tunnel(i)->number + 1) }.draw(x + t_n + (50 * i), y);
	y += h_font * 2;

	if (bat_nearby(room_)) {
		Message{ "hear_bat" }.draw(x, y); y += h_font;
	}
	if (pit_nearby(room_)) {
		Message{ "feel_breeze" }.draw(x, y); y += h_font;
	}
	if (wumpus_nearby(room_)) {
		Message{ "smell_wumpus" }.draw(x, y); y += h_font;
	}
}
void general_message::room(Room* r)
{
	if (!r) error("general_message; Room required;");
	room_ = r;
	/*
	char lines = 3;
	lines += bat_nearby(room_);
	lines += pit_nearby(room_);
	lines += wumpus_nearby(room_);
	return lines;*/
}





void arrow_message::draw(int x, int y)
{
	Message::draw(x, y);
	if (numbers.size())
	{
		Message{ to_string(numbers[0]).c_str() }.draw(x + r_x, y);

		vector<int> ns{ numbers };
		numbers = {};
		for (int n{ 1 }; n < ns.size(); ++n)
			numbers.push_back(ns[n]);
	}
}