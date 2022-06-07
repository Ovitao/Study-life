#pragma once
#include "../../../Custom Header Files/Graphical_Headers/Graph.h"

/*5.6.22
*/


using namespace Graph_lib;

class Arrow : public Shape
{
public:
	Arrow(Point target, Point pp, int size = 10);

	const Point& target_point() const { return point(3); }
	const Point& end_point() const { return point(0); }
	const int& size() const { return s; }

	void move_target_point(int x, int y);
	void move_end_point(int x, int y);

	void draw_lines() const;
private:
	int s{ 0 };
};




double cos(double a, double b);

double Hypotenuse(double a, double b);

double line_length(Point first, Point end);

struct point_double {
	point_double()
	{}
	point_double(double xx, double yy)
		:x{ xx }, y{ yy }{}
	double x{};
	double y{};
};

double line_length(point_double first, point_double end);
//----------------------------------------------------------------------------------------
Point point_on_line(Point start, Point direction, int distance);
//----------------------------------------------------------------------------------------

// right angle to the line
Point right_angle(Point angle_point, Point line_point, int length);
//----------------------------------------------------------------------------------------



// point_double for more precision in further calculations
point_double point_on_line(point_double start, point_double direction, int distance);

// point_double for more precision in further calculations
point_double right_angle(point_double angle_point, point_double line_point, int length);

  





//----------------------------------------------------------------------------------------------------


// 5.11.22

Point angle_to_line(Point angle_point, Point p2, int angle, int length);
double pi();

double as_degree(double);
double as_radian(double);