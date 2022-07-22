// 15.7.22
#include "Hunt_The_Wumpus.h"
#include<time.h>
#include<chrono>

//----------------------------------------------

Input_Win::Input_Win(Fl_Window* flw_owner)
	:Fl_Window{ flw_owner->x(),flw_owner->y() + flw_owner->h() - 55,215,55,"Input" },
	owner{ flw_owner },
	input_box{ 10,10,150,35,">" },
	Do_b{ 170,10,35,35,"Do!" }
{
	add(input_box);
	add(Do_b);
	Do_b.callback([](Fl_Widget* wd, void* hw) { reference_to<Wump_Game>(hw).make_turn(); }, owner);
	end();
}
string Input_Win::get() const
{
	return input_box.value();
}
//----------------------------------------------
void Messages::draw()
{
	short int y = 0;
	for (char i = 0; i < msgs.size(); i++)
		while (printtimes[i]) {
			printtimes[i]--;
			msgs[i].first->draw(0, y);
			y += msgs[i].second * h_font;
		}
}
Messages::~Messages()
{
	for (auto m : msgs) delete m.first;
}

//----------------------------------------------
void state_of_cave(Wump_Game* wg);
//----------------------------------------------
bool is_null_or_repeated_connection(Cave* cv)
{
	for (size_t i = 0; i < cv->number_of_rooms; i++)
		if (nulls(cv->room(i)) || repeated_connection(cv->room(i))) return true;

	return false;
}
int mid_lvl_odd(int n)
{
	return n + n + 5;
}
int mid_lvl_even(int n)
{
	return n + n + 6;
}
void set_rand_time_seed()
{
	time_t tt;
	time(&tt);
	seed_randint(tt);
}
void generate_connections(Cave* cv)
{
	for (size_t i = 0; i < cv->number_of_rooms; i++)
		cv->room(i)->number = i;

	connect(cv->room(4), cv->room(0));
	connect(cv->room(14), cv->room(5));
	connect(cv->room(19), cv->room(15));

	for (char n = 0; n < cv->number_of_rooms - 1; n++)
		connect(cv->room(n), cv->room(n + 1));

	for (char n = 0; n < 4; n++)
		connect(cv->room(n), cv->room(mid_lvl_odd(n + 1)));

	for (char n = 16; n < cv->number_of_rooms; n++)
		connect(cv->room(n), cv->room(mid_lvl_even(n - 16)));

	if (is_null_or_repeated_connection(cv)) error("null connection");
}
void Cave::draw()
{
	if (visible())
	{
		Fl_Window::draw();

		child(0)->redraw();
		fl_color(grey64);

		fl_line_style(Line_style::solid, 10);
		fl_circle(center_of_cave.x, center_of_cave.y, radius1);
		fl_circle(center_of_cave.x, center_of_cave.y, radius2);
		fl_circle(center_of_cave.x, center_of_cave.y, radius3);


		fl_line(rooms[4].point().x, rooms[4].point().y, rooms[5].point().x, rooms[5].point().y);
		fl_line(rooms[14].point().x, rooms[14].point().y, rooms[15].point().x, rooms[15].point().y);

		for (char n = 0; n < 4; n++)
			fl_line(rooms[n].point().x, rooms[n].point().y, rooms[mid_lvl_odd(n + 1)].point().x, rooms[mid_lvl_odd(n + 1)].point().y);

		for (char n = 16; n < number_of_rooms; n++)
			fl_line(rooms[n].point().x, rooms[n].point().y, rooms[mid_lvl_even(n - 16)].point().x, rooms[mid_lvl_even(n - 16)].point().y);

		fl_line_style(0);
		for (char i = 0; i < 20; i++)
			rooms[i].draw();
	}
}
Room* Cave::room(int n) { 
	if (n < number_of_rooms && 0 <= n)
		return &rooms[n];
	error("room: out of range", n);	
}
void generate_pits(Cave* cv)
{
	Room* room;
	for (size_t i = 0; i < cv->number_of_pits; i++)
	{
		while ((room = cv->room(randint(0, cv->number_of_rooms - 1)))->has_pit);
		room->has_pit = true;
	}
}
void place_bats(Cave* cv)
{
	Room* room;
	for (size_t i = 0; i < cv->number_of_bats; i++)
	{
		while ((room = cv->room(randint(0, cv->number_of_rooms - 1)))->bat_here);
		room->bat_here = true;
	}
}
Cave::~Cave()
{
	if (rooms) delete[]rooms;
}
void undescover(Cave_Map* cv)
{
	for (size_t i = 0; i < cv->number_of_rooms; i++)
		cv->mark_map(i, Cave_Map::markers::unknown);
}
void remove_bats(Cave* cv)
{
	for (size_t i = 0; i < cv->number_of_rooms; i++)
		cv->room(i)->bat_here = false;
}
void remove_pits(Cave* cv)
{
	for (size_t i = 0; i < cv->number_of_rooms; i++)
		cv->room(i)->has_pit = false;
}
void generate_points(Cave* cv)
{
	constexpr double circle = 6.28319;
	constexpr double pentagon_gap = circle / 5.0;
	constexpr double tentagon_gap = circle / 10.0;

	double radians_ = 0;
	for (char i = 0; ; ++i )
	{
		const int yy = round(cos(radians_) * cv->radius1);
		const int xx = round(sin(radians_) * cv->radius1);
		cv->room(i)->set_point({ cv->center_of_cave.x + xx, cv->center_of_cave.y - yy });
		if (i == 4) break; 
		radians_ += pentagon_gap;
	}
	for (char i = 5; ; i++)
	{
		const int yy = round(cos(radians_) * cv->radius2);
		const int xx = round(sin(radians_) * cv->radius2);
		cv->room(i)->set_point({ cv->center_of_cave.x + xx, cv->center_of_cave.y - yy });
		if (i == 14) break;
		radians_ += tentagon_gap;
	}
	for (char i = 15; i < 20; i++)
	{
		const int yy = round(cos(radians_) * cv->radius3);
		const int xx = round(sin(radians_) * cv->radius3);
		cv->room(i)->set_point({ cv->center_of_cave.x + xx, cv->center_of_cave.y - yy });
		radians_ += pentagon_gap;
	}
}
Cave::Cave(int pits, int bats)
	:Fl_Window{300 ,200, 1200,600, "Hunt The Wumpus"},
	center_of_cave{ int(w() * 0.75),int(h() / 2) },
	//rooms{ new Room[number_of_rooms]{} },
	number_of_bats{ bats == -10 ? int(number_of_rooms * .25) : bats },
	number_of_pits{ pits ==-10 ? int(number_of_rooms *.15): pits },
	number_of_rooms{ 20 }
{	
	add(bgimage);
	end();
	if (number_of_bats < 0) error("number of bats is negative");
	if (number_of_bats > number_of_rooms) error("don't place more bats than rooms");

	if(number_of_pits > number_of_rooms || 0 > number_of_pits) error("number_of_pits out of [0:number_of_rooms] renge");
	set_rand_time_seed();
	generate_connections(this);
	generate_points(this);
	generate_pits(this);
	place_bats(this);
}
//-----------------------------------------------------
Room* choose_random_path(Room* r)
{
	return r->tunnel(randint(0, 2));
}
void connect_with_arc(Room* r1, Room* r2, Point center)
{
	const double sdX1 = r1->point().x - center.x;
	const double sdY1 = center.y - r1->point().y;

	const double sideX2 = r2->point().x - center.x;
	const double sideY2 = center.y - r2->point().y;

	double degree1 = 0;
	double degree2 = 0;
	constexpr double degree_per_radian = 57.2958;

	if (sdX1)
	{
		degree1 = sdY1 / sdX1;
		degree1 = atan(degree1);
		degree1 *= degree_per_radian;
	}
	if (sideX2)
	{
		degree2 = sideY2 / sideX2;
		degree2 = atan(degree2);
		degree2 *= degree_per_radian;
	}


	const double radius = sqrt(pow(sdX1, 2) + pow(sdY1, 2));
	constexpr double d90 = 90;

	if (r1->point().x < center.x)
		degree1 += d90 * 2;
	else
		if (r1->point().x == center.x)
		{
			degree1 += d90;
			if (r1->point().y > center.y)
				degree1 += d90 * 2;
		}
		else
			if (r1->point().y > center.y)
				degree1 += d90 * 4;

	if (r2->point().x < center.x)
		degree2 += d90 * 2;
	else
		if (r2->point().x == center.x)
		{
			degree2 += d90;
			if (r2->point().y > center.y)
				degree2 += d90 * 2;
		}
		else
			if (r2->point().y > center.y)
				degree2 += d90 * 4;


	fl_arc(center.x - radius, center.y - radius, radius * 2, radius * 2, degree2, degree1);
}
bool the_same_radius(Point c1, Point c2, Point center)
{
	return (sqrt(pow(center.x - c1.x, 2) + pow(center.y - c1.y, 2)) == sqrt(pow(center.x - c2.x, 2) + pow(center.y - c2.y, 2)));
}
bool connect_draw_line_check(Room* r1, Room* r2, Cave_Map* hc)
{
	return (r1->fill_color().as_int() != Cave_Map::markers::unknown && r2->fill_color().as_int() != Cave_Map::markers::unknown)
		|| r1 == hc->hunter_room() || r2 == hc->hunter_room();
}
bool room_draw_check(Room* r1, Cave_Map* hc)
{
	return r1->fill_color().as_int() != Cave_Map::markers::unknown||
		r1 == hc->hunter_room() ||
		r1->tunnel(0) == hc->hunter_room()
		|| r1->tunnel(1) == hc->hunter_room() || r1->tunnel(2) == hc->hunter_room();
}
void wumpus_moves(Cave_Map* rc)
//moves into adjoining room
{
	rc->wumpus_goes_to(choose_random_path(rc->wumpus_room())->number);
}
void take_hunter_in_other_room(Cave_Map* rc)
//moves into adjoining room
{
	rc->hunter_room(choose_random_path(rc->hunter_room())->number);
}
void Cave_Map::draw()
{
	if (visible())
	{
		if (draw_whole_cave) return Cave::draw();
		Fl_Window::draw();

		fl_color(grey64);
		fl_line_style(Line_style::solid, 10);

		//lines
		if (connect_draw_line_check(room(4), room(5), this))
			fl_line(room(4)->point().x, room(4)->point().y, room(5)->point().x, room(5)->point().y);
		if (connect_draw_line_check(room(14), room(15), this))
			fl_line(room(14)->point().x, room(14)->point().y, room(15)->point().x, room(15)->point().y);

		for (char n = 0; n < 4; n++)
		{
			Room* r1 = room(n);
			Room* r2 = room(mid_lvl_odd(n + 1));
			if (connect_draw_line_check(r1, r2, this))
				fl_line(r1->point().x, r1->point().y, r2->point().x, r2->point().y);
		}

		for (char n = 16; n < number_of_rooms; n++)
		{
			Room* r1 = room(n);
			Room* r2 = room(mid_lvl_even(n - 16));
			if (connect_draw_line_check(r1, r2, this))
				fl_line(r1->point().x, r1->point().y, r2->point().x, r2->point().y);
		}


		// arcs
		for (char i = 1; i < 20; i++)
			if (i != 15 && i != 5 && connect_draw_line_check(room(i - 1), room(i), this))
				connect_with_arc(room(i - 1), room(i), center_of_cave);

		if (connect_draw_line_check(room(4), room(0), this))
			connect_with_arc(room(4), room(0), center_of_cave);
		if (connect_draw_line_check(room(14), room(5), this))
			connect_with_arc(room(14), room(5), center_of_cave);
		if (connect_draw_line_check(room(19), room(15), this))
			connect_with_arc(room(19), room(15), center_of_cave);


		for (char i = 0; i < 20; i++)
			if (room_draw_check(room(i), this)) room(i)->draw();
		fl_line_style(0);
	}
}
void Cave_Map::mark_map(unsigned int r, markers mm)
{
	room_checker(r);
	room(r)->set_fill_color(mm);
}
bool Cave_Map::wumpus_goes_to(unsigned int n)
{
	room_checker(n);
	wumpus_room()->wumpus_here = false;
	wumpus_in_room = n;
	wumpus_room()->wumpus_here = true;
	return true;
}
void Cave_Map::hunter_room(unsigned int n)
{
	room_checker(n);
	hunter_in_room = n;
}
// 
void place_hunter(Cave_Map* hunt_c)
{
	int n{ 0 };
	while ((n = randint(0, hunt_c->number_of_rooms - 1)) == hunt_c->wumpus_room()->number ||
		hunt_c->room(n)->has_pit ||
		hunt_c->room(n)->bat_here);
	hunt_c->hunter_room(n);
	hunt_c->mark_map(n, Cave_Map::markers::discovered);
}
Cave_Map::Cave_Map()
	:Cave{},
	wumpus_in_room{ randint(0, number_of_rooms-1) }
{
	wumpus_room()->wumpus_here=true;
}
//-------------------------------------------------------
void bat_flies_in_other_room(Room* rr)
{
	Room* r;
	while ((r = choose_random_path(rr))->bat_here == true || r == rr);
	r->bat_here = true;

}
Room* shoot_into_room(Room* n_from, Room* aim)
{
	if (aim && is_connected(n_from, aim)) return aim;
	else return choose_random_path(n_from);
}
shoot_sequence make_shoot_sequence(Wump_Game* hc, istringstream& is)
{
	shoot_sequence rss;

	int next_r;
	if (!(is >> next_r))
		throw err_num(Messages::err::type_number_of_room_after_s);

	Room* _room = hc->hunter_room();
	Room* n_room;
	char ch;
	for (char i = 0; i < 3; i++)
	{
		if (next_r > 0 && next_r <= hc->number_of_rooms)
			n_room = hc->room(next_r - 1);
		else n_room = nullptr;

		rss.r(i).second = (!n_room || !is_connected(_room, n_room));
		_room = shoot_into_room(_room, n_room);
		rss.r(i).first = _room;

		if (!(is >> ch)) break;
		if (!(is >> next_r)) throw err_num(Messages::err::type_number_of_rooms_combination);
	}
	return rss;
}
void Wump_Game::shoot(shoot_sequence rss)
{
	for (char i = 0; rss.r(i).first && i < 3; i++)
	{
		if (rss.r(i).second)
			msgs.print_message(Messages::prompt::arow_ricochets, rss.r(i).first);
		else
			msgs.print_message(Messages::prompt::arow_flyes, rss.r(i).first);

		if (rss.r(i).first->wumpus_here)
		{
			end_game = true;
			msgs.print_message(Messages::prompt::slain);
			msgs.print_message(Messages::prompt::end_game);
			return;
		}
	}
	wumpus_moves(this);
	msgs.print_message(Messages::prompt::miss);
}
istream& nearby_room_reader::get_room_number(istream& is)
{
	if (is >> number){
		number -= 1;
		if(!disable_check) nearby_room_checker::operator()(hunter_room, cave->room(number));
	}
	return is;
}
bool Wump_Game::recognize_cm_and_run(string s)
{
	istringstream is{ s };
	string cm;
	for (char c; is.get(c) && !isdigit(c) && !isspace(c); )cm += c;
	is.unget();
	size_t n = 0;
	for (n; n < commands.size() && cm != commands[n]; n++);

	nearby_room_reader crc{ hunter_room(),this };
	switch (n)
	{
	case 0:
		if(!crc.get_room_number(is)) throw err_num(Messages::err::choose_room_move);
		hunter_room(crc.number);
		break;
	case 1:
	{
		char c;
		if (!(is >> c)) throw err_num(Messages::choose_room_shoot);
		is.unget();
		shoot(make_shoot_sequence(this, is));
		break;
	}
	case 2:
		state_of_cave(this);
		break;
	case 3:
		draw_whole_cave =true ;
		break;
	case 4:
		crc.disable_check = draw_whole_cave;
		if (!crc.get_room_number(is)) throw err_num(Messages::err::choose_room);
		crc.disable_check = false;
		mark_map(crc.number, markers::bat);
		break;
	case 5:
		crc.disable_check = draw_whole_cave;
		if (!crc.get_room_number(is)) throw err_num(Messages::err::choose_room);
		crc.disable_check = false;
		mark_map(crc.number, markers::pit);
		break;
	case 6:
		crc.disable_check = draw_whole_cave;
		if (!crc.get_room_number(is)) throw err_num(Messages::err::choose_room);
		crc.disable_check = false;
		mark_map(crc.number, markers::something);
		break;
	case 7:
		crc.disable_check = true;
		if (!crc.get_room_number(is)) throw err_num(Messages::err::choose_room);
		crc.disable_check = false;
		if(room(crc.number)->fill_color().as_int()!= markers::discovered) mark_map(crc.number, markers::unknown);
		break;
	default:
		throw err_num(Messages::err::no_command);
	}
	return 1;
}

void Wump_Game::determine_state()
{
	if (hunter_room() == wumpus_room()) {
		end_game = true;
		msgs.print_message(Messages::prompt::ate_you);
		msgs.print_message(Messages::prompt::end_game);
	}
	else
	if (hunter_room()->bat_here) {
		hunter_room()->bat_here = false;
		take_hunter_in_other_room(this);
		bat_flies_in_other_room(hunter_room());
		msgs.print_message(Messages::prompt::bat_catches_you);

		determine_state();
	}
	else
	if (hunter_room()->has_pit)
	{
		end_game = true;
		msgs.print_message(Messages::prompt::fell_into_pit);
		msgs.print_message(Messages::prompt::end_game);
	}
}
void Wump_Game::new_game()
{
	undescover(this);
	remove_bats(this);
	place_bats(this);
	//remove_pits(this);
	//generate_pits(this);
	wumpus_goes_to(randint(0, number_of_rooms - 1));
	place_hunter(this);
	
	in_box_window.show();
	end_game = false;
	draw_whole_cave = false;
	msgs.print_message(Messages::prompt::general_prompt, hunter_room());
	redraw();
}
Wump_Game::Wump_Game()
	:new_game_b{ new Fl_Button{ w() -70 ,0 ,70,20,"New Game" } },
	in_box_window{ this }
{
	add(new_game_b.get());
	new_game_b->callback([](Fl_Widget* wd, void* hw) { reference_to<Wump_Game>(hw).new_game(); }, this);
	end();
	msgs.print_message(Messages::rules);
//	state_of_cave(this);
}
void Wump_Game::make_turn()
{
	if (!end_game)
	try
	{
		recognize_cm_and_run(in_box_window.get());
		determine_state();
		if (end_game)
		{
			if(hunter_room()==wumpus_room()|| hunter_room()->has_pit) 
				mark_map(hunter_room()->number, markers::i_died_here);
			else
				mark_map(wumpus_room()->number, markers::here_the_wumpus);
			in_box_window.hide();
		}
		else mark_map(hunter_room()->number, markers::discovered);
	}
	catch (const std::exception& e)
	{
		cerr <<"Error: " <<e.what() <<".\n";
	}
	catch (const err_num& e)
	{
		msgs.print_err(Messages::err::ops);
		msgs.print_err(e.what());
	}
	if (!end_game)	msgs.print_message(Messages::prompt::general_prompt, hunter_room());
	redraw();
}
void Wump_Game::run()
{
	show();
	gui_main();
}
void Wump_Game::draw()
{
	if (visible())
	{
		Cave_Map::draw();
		msgs.draw();
	}
}
//------------------------------------------------------------------
void state_of_cave(Wump_Game*wg)
{
	cout << "In code number of room -1\n";
	for (size_t i = 0; i < wg->number_of_rooms; i++)
	{
		cout << "Room " << wg->room(i)->number+1 << " -> ";
		if (wg->room(i)->tunnel(0))cout << wg->room(i)->tunnel(0)->number + 1 << " ";
		else cout << " null ";
		if (wg->room(i)->tunnel(1))cout << wg->room(i)->tunnel(1)->number + 1;
		else cout << " null ";
		if (wg->room(i)->tunnel(2))cout << " " << wg->room(i)->tunnel(2)->number + 1 << '.';
		else cout << " null ";
		if (wg->room(i)->has_pit) cout << " has pit. ";
		if (wg->room(i)->bat_here) cout << " bat here. ";
		if (wg->room(i)->wumpus_here) cout << " wampus here. ";
		cout << '\n';
	}
}
//------------------------------------------------------------------





int main()
try
{
	Wump_Game().run();
}
catch (const std::exception& e)
{
	cerr << e.what() << '\n';
	keep_window_open("Error: " + string(e.what()));
}
