/*
8/12/22

Here are the angle() function that gives Point with chosen degree and length to given line:
	degree is double;
	works with negative degree and with degree over 360;
	works with negative length (inversion).

and test environment for the function:
	it is a window with buttons to adjust the degree, length and to rotate the line on which the angle is calculated;
	rotation includes 8 positions for angle point , it places the point on 4 planes and 4 axes (conterclockwise from 0 to 7'th button).
*/
#include "../../../Custom Header Files/Graphical_Headers/Gui.h"
using namespace Graph_lib;




// radian relative to the positive x-axis with 'a_center' as point 0.
double get_radian(const Point& a_center, const Point& b)
{
	const double side_X = b.x - a_center.x;
	const double side_Y = a_center.y - b.y;
	double radian = 0;

	if (side_X) 
		radian = atan(side_Y / side_X);

	constexpr double right_angle{ 1.57079633 };
	if (b.x < a_center.x)
		radian += right_angle * 2; // in some planes (2'd and 4'th) angle is calculated for other angle of triangle (rotated) and result is negative. we can get the target angle adding one additional right angle to the value. 
	else
		if (b.x == a_center.x)
		{
			radian += right_angle;
			if (b.y > a_center.y)
				radian += right_angle * 2;
		}
		else
			if (b.y > a_center.y)
				radian += right_angle * 4; 
	return radian;
}
/*
	angle from Point a.
	works with negative length (line is drawn on the other side)(inversion)
	works with negative degree and with degree over 360
*/
Point angle(double degree,int length, Point a, Point b)
{
	constexpr double radian_per_degree = 0.0174533;
	double radian{ degree * radian_per_degree };
	radian += get_radian(a, b);

	int x = round(cos(radian) * length);
	int y = round(sin(radian) * length);

	return { a.x + x ,a.y - y };
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Test enviroment

struct rotated_line :public Shape
{
	rotated_line(Point p)
		:lines{
			{Point{p.x - 30,p.y},Point{p.x + 30,p.y}},
			{Point{p.x - 20,p.y + 20},Point{p.x + 20,p.y - 20}},
			{Point{p.x,p.y + 30},Point{p.x,p.y - 30}},
			{Point{p.x + 20,p.y + 20},Point{p.x - 20,p.y - 20}},
			{Point{p.x + 30,p.y},Point{p.x - 30,p.y}},
			{Point{p.x + 20,p.y - 20},Point{p.x - 20,p.y + 20}},
			{Point{p.x,p.y - 30},Point{p.x,p.y + 30}},
			{Point{p.x - 20,p.y - 20},Point{p.x + 20,p.y + 20}}
	}
	{
		set_color(Color::black);
		add(lines[0].first);
		add(lines[0].second);
	}
	void set_plane(char c)
	{
		set_point(0, lines[c].first);
		set_point(1, lines[c].second);
	}
private:
	vector<pair<Point, Point>> lines;
};
struct angle_line :public Shape
{
	angle_line(rotated_line& l)
		:rl{l}
	{
		set_color(Color::dark_green);
		//add(rl.point(0), rl.point(0));
	//	add(angle(degree, 70, rl.point(0), rl.point(0)));
	}
	double degree{ 0 };
	
	int length{ 70 };
	void draw_lines()const override
	{
		Point p = angle(degree, length, rl.point(0), rl.point(1));
		fl_line(rl.point(0).x, rl.point(0).y, p.x, p.y);
	}
private:
	rotated_line& rl;
};
class test_angle_enviroment : public Window
{
public:
	test_angle_enviroment()
		:Window{ 200, 300, "Test" },
		pl1{ 50,0,20,20,"0" },
		pl2{ pl1.x() + pl1.w(),0,pl1.w(),pl1.w(),"1" },
		pl3{ pl2.x() + pl1.w(),0,pl1.w(),pl1.w(),"2" },
		pl4{ pl3.x() + pl1.w(),0,pl1.w(),pl1.w(),"3" },
		pl5{ pl4.x() + pl1.w(),0,pl1.w(),pl1.w(),"4" },
		pl6{ pl5.x() + pl1.w(),0,pl1.w(),pl1.w(),"5" },
		pl7{ pl6.x() + pl1.w(),0,pl1.w(),pl1.w(),"6" },
		pl8{ pl7.x() + pl1.w(),0,pl1.w(),pl1.w(),"7" },
		plus_1{ 0,40,pl1.w(),pl1.w(),"+1" },
		minus_1{ 0,plus_1.y() + plus_1.h(),pl1.w(),pl1.w(),"-1" },
		plus_5{ plus_1.x()+ plus_1.w(),plus_1.y(),pl1.w(),pl1.w(),"+5" },
		minus_5{ plus_5.x(),plus_5.y() + plus_5.h(),pl1.w(),pl1.w(),"-5" },
		plus_10{ plus_5.x() + plus_1.w(),plus_1.y(),pl1.w()+5,pl1.w(),"+10" },
		minus_10{ plus_10.x(),plus_5.y() + plus_5.h(),pl1.w() + 5,pl1.w(),"-10" },
		degree{0,minus_1.y()+ minus_1.h(),65,20,""},
		l_plus_10{ 0,degree.y() + degree.h() + 14,plus_10.w(),pl1.w(),"+10" },
		l_minus_10{ 0 ,l_plus_10.y() + l_plus_10.h(),l_plus_10.w(),pl1.w(),"-10" }
	{
		pl1.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(0); }, this);
		pl2.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(1); }, this);
		pl3.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(2); }, this);
		pl4.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(3); }, this);
		pl5.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(4); }, this);
		pl6.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(5); }, this);
		pl7.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(6); }, this);
		pl8.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).set_plane(7); }, this);
		plus_1.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(1); }, this);
		minus_1.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(-1); }, this);
		plus_5.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(5); }, this);
		minus_5.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(-5); }, this);
		plus_10.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(10); }, this);
		minus_10.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_degree(-10); }, this);
		l_plus_10.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_length(10); }, this);
		l_minus_10.callback([](Fl_Widget*, void* vd) { reference_to<test_angle_enviroment>(vd).change_length(-10); }, this);
		add(plus_1);	add(minus_1); add(degree);
		add(plus_5);	add(minus_5); 		add(plus_10);	add(minus_10);
		add(l_plus_10);	add(l_minus_10);
		attach(rl);	attach(al);
		add(pl1);	add(pl2);	add(pl3);
		add(pl4);	add(pl5);	add(pl6);
		add(pl7);	add(pl8);
	}
	void draw();
private:

	void set_plane(char c)
	{
		rl.set_plane(c);
		redraw();
	}
	void change_degree(double d)
	{
		al.degree+=d;
		redraw();
	}
	void change_length(int d)
	{
		al.length += d;
		redraw();
	}
	rotated_line rl{ {200,120} };
	angle_line al{ rl };
	Fl_Button pl1;
	Fl_Button pl2;
	Fl_Button pl3;
	Fl_Button pl4;
	Fl_Button pl5;
	Fl_Button pl6;
	Fl_Button pl7;
	Fl_Button pl8;
	Fl_Button plus_1;
	Fl_Button minus_1;
	Fl_Button plus_5;
	Fl_Button minus_5;
	Fl_Button plus_10;
	Fl_Button minus_10;


	Fl_Output degree;
	Fl_Button l_plus_10;
	Fl_Button l_minus_10;
};

void test_angle_enviroment::draw()
{
	if (visible()) 
	{
		degree.value(to_string(al.degree).c_str());
		Window::draw();
		fl_draw("Planes:", 0, 14);
		fl_draw("Degree:", 0, 35);
		fl_draw("Length:", l_plus_10.x(), l_plus_10.y()-5);
	}
}
int main()
try
{
	

	test_angle_enviroment ts;

	
	Fl::run();
}
catch (const std::exception& e)
{
	cerr << e.what();
	keep_window_open("~");
}
