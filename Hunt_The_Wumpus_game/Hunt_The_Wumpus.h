// 15.7.22
/*
	Game uses the FLTK library.
	Input through text box.
	The map of the cave is schlegel diagram of dodecahedron.
	As you play it draws part of cave that has already been discovered.
	You can mark visible rooms with commands. (The help section is included at the start of the game).
	With the command "show_cave" you can ask the game to draw the entire cave;
	You can see the location of the hazards with the command "state_of_cave" (this will be displayed in the console);

	Features:	-If you enter a room with a bottomless pit and a Giant Bat is there, it will drag you to another room and you won't die;
				-If whole cave is drawn, you can mark any room.
				-If you shoot in room that is not nearby.  The arrow will ricochet into a random room and a suitable message will be shown. 
				-After the giant bat catches you, it will fly to another room. the room where the bat caught you will not be marked as discovered.
				-The shooting range is up to 3 rooms;

*/



//#include "../../../../Custom Header Files/Graphical_Headers/Gui.h"
#include "Room.h"

struct BackGround_Image : Fl_Widget // widget for image to draw it under the "New Game" button
{
	BackGround_Image(string s)
		:image{ s.c_str() }, Fl_Widget{ 0,0,1200,600 }{}

	void draw() override
	{
		image.draw(0, 0);
	}

	Fl_JPEG_Image image;
};
//-----------------------------------------

class Input_Win : public Fl_Window
{
public:
	using Fl_Window::Fl_Window;
	Input_Win(Fl_Window* flw_owner);
	string get() const;
private:
	Fl_Input input_box;
	Fl_Button Do_b;
	Fl_Window* owner;
};
//-----------------------------------------
class Cave : public Fl_Window
{
public:
	//using Fl_Window::Fl_Window;
	Cave(int numer_of_pits=-10, int number_of_bats = -10); // numer_of_pits by default 15% of rooms_number, number_of_bats by default 25%

	const int number_of_rooms;
	const int number_of_pits;
	const int number_of_bats;
	Room* room(int n);
	void draw() override;
	virtual ~Cave();

	const Point center_of_cave;
	const int radius1 = 250, radius2 = 170, radius3 = 90;
protected:
	
	BackGround_Image bgimage{ "back_ground.jpg" };
private:
	Room rooms[20];
};
//-----------------------------------------

struct Messages
{
	enum err{
		ops,
		choose_room_move,
		choose_room_shoot,
		choose_room,
		no_room_nearby,
		no_command,
		type_number_of_rooms_combination,
		type_number_of_room_after_s
	};
	enum prompt {
		rules = 8,
		bat_catches_you,
		arow_flyes,
		arow_ricochets,
		miss,
		ate_you,
		slain,
		fell_into_pit,
		end_game,
		general_prompt
	};

	void draw();
	
	void print_message(unsigned int n,Room*r=0)
	{
		if (n < msgs.size())
		{
			++printtimes[n];
			msgs[n].first->room(r);
			return;
		}
		error("range error print_message",n);
	}
	void print_err(unsigned int n)
	{
		if(n< rules)
			++printtimes[n];
		else
		error("print_err range error",n);
	}
	~Messages();
private:
	static constexpr char h_font = 36;
	
	vector<pair<Message*,const char>> msgs
	{
		{new Message{	"ops"} ,1},
		{new Message{	"choose_room_m"},1},
		{new Message{	"choose_room_s"},1},
		{new Message{	"choose_room"},1},
		{new Message{	"no_room"},1},
		{new Message{	"no_command"},3},
		{new Message{	"num_comb"},2},
		{new Message{	"type_num_of_roomafter_s"},2},//7
		
		{new Message{	"rules"},0},
		{new Message{	"bat_catches_you"},2},
		{new arrow_message{	"arow1"},1},
		{new arrow_message{	"arow2"},1},
		{new Message{	"miss"},2},
		{new Message{	"ate_you"},2},
		{new Message{	"slain" },1},
		{new Message{	"fell_into_pit"},2},
		{new Message{	"end_game"},2},
		{new general_message,3} // 17
	};
	vector<char> printtimes{ vector<char>(msgs.size(), 0) };
};
//-----------------------------------------

class Cave_Map :public Cave
{
public:
	enum markers
	{
		unknown = FL_BLACK,
		something = FL_YELLOW,
		bat = 1426063360, // fl_darker(FL_DARK_RED)
	//	maybe_pit = FL_CYAN,

		here_the_wumpus = FL_RED,
		i_died_here = FL_BLUE,

		discovered = FL_WHITE,
	//	maybe_bat  = FL_DARK_RED,
		pit = 6378752,  // fl_darker(FL_DARK_CYAN)
	};
	
	void mark_map(unsigned int room, markers mm);
	
	using Cave::Cave;
	Cave_Map();

	bool wumpus_goes_to(unsigned int);
	void hunter_room(unsigned int);

	Room* hunter_room() { return room(hunter_in_room); }
	Room* wumpus_room() {	return room(wumpus_in_room);	}
	void draw() override;
protected:
	bool draw_whole_cave{ true };
private:
	int hunter_in_room;
	int wumpus_in_room;
};

//-----------------------------------------


class Wump_Game :public Cave_Map
{
public:
	using Cave_Map::Cave_Map;
	Wump_Game();
	

	void run();
	void make_turn();
	void draw() override;
protected:
	istream* istr{ &cin };
	istream& instream{ *istr };

	bool end_game = false;

	void determine_state();
	bool recognize_cm_and_run(string);

	unique_ptr<Fl_Button> new_game_b;
	Input_Win in_box_window;
	void new_game();
private:
	Messages msgs;
	void shoot(shoot_sequence);
	vector<string>commands{ "m","s", // action
		"state_of_cave", "show_cave", // utility
		"bat","pit","sth","un" };// map markers 
};
//-----------------------------------------

struct nearby_room_checker
{
	void operator()(Room* r1, Room* r2) const
	{
		if (!is_connected(r1, r2)) throw err_num(Messages::err::no_room_nearby);
	}
};

struct nearby_room_reader: nearby_room_checker
{
	nearby_room_reader(Room* hunter_room, Cave* c)
		: hunter_room{ hunter_room }, cave{ c }
	{	}
	bool disable_check = false;
	istream& get_room_number(istream& is);
	int number{ -1 };
private:
	Room* hunter_room;
	Cave* cave;
};