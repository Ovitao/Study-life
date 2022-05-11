#pragma once
#include "../../../Custom Header Files/Graphical_Headers/Simple_window.h"
/*5.6.22
*/





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
Point right_angle(Point angle_point, Point second_point, int length);
//----------------------------------------------------------------------------------------



// point_double for more precision in further calculations
point_double point_on_line(point_double start, point_double direction, int distance);

// point_double for more precision in further calculations
point_double right_angle(point_double angle_point, point_double second_point, int length);

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


// 5.11.22

Point angle_to_line(Point angle_point, Point p2, int C1_angle, int length);

double as_degree(double);
double as_radian(double);