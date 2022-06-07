/*5.6.22
Here are functions that can calculate:
 triangle arrow at the end of a line. (uses double_point to calculate and attaches Point (int) to window
 coordinate of a point for perpendicular to end point of a chosen line (double or int point) (select the direction with the sign)
 coordinate of a point on any abstract line that can lye on 2 points (beyond points too). (double or int point) (select the direction with the sign)
 length betwean 2 points (double)
*/
// 6.23.22 Arrow now is class



#include "angles.h"

double cos(double a, double b) {
	return a / b;
}

double Hypotenuse(double a, double b) {
	double c = pow(a, 2) + pow(b, 2);
	c = sqrt(c);
	return c;
}

double line_length(Point first, Point end)
{
	double a = abs(first.y - end.y);
	double b = abs(first.x - end.x);
	if (a == 0) return b; // lenght == x1-x2 coordinate, horizontal line
	if (b == 0) return a; // lenght == y1-y2 coordinate, vertical line

	return Hypotenuse(a, b);
}

double line_length(point_double first, point_double end)
{
	double a = abs(first.y - end.y);
	double b = abs(first.x - end.x);
	if (a == 0) return b; // lenght == x1-x2 coordinate, horizontal line
	if (b == 0) return a; // lenght == y1-y2 coordinate, vertical line

	return Hypotenuse(a, b);
}
//----------------------------------------------------------------------------------------
Point point_on_line(Point start, Point direction, int distance)
// finds point on any abstract line of 2 collinear points. at 'start' 'distance' == 0
// use negative 'distance' for finding point on the line beyond the 'start'
{
	double a = abs(start.y - direction.y); // Opposite side of larger right triangle formed by the 'start' and 'direction' points with right angle on the start.x coordinate horizontal line
	double b = abs(start.x - direction.x); // Adjacent side of larger right triangle formed by the 'start' and 'direction' points with right angle on the start.x coordinate horizontal line
	if (a == 0) { return { start.x += start.x < direction.x ? distance : -distance, start.y }; }
	if (b == 0) { return { start.x, start.y += start.y < direction.y ? distance : -distance }; }

	short int sin90 = 1;
	double sinB = (a * sin90) / Hypotenuse(b, a); // angle of a larger right triangle. the same angle for a smaller triangle. angle formed by start.x coordinate horizontal line and line on the 'start' and 'direction' points
	double y = sinB * abs(distance); //(change to y coordinate). Opposite of smaller right triangle, where 'distance' is Hypotenuse.  (lying on the line on the 'start' and 'direction' points).
	double x = pow(distance, 2) - pow(y, 2); // (change to x coordinate). Adjacent of smaller right triangle, where 'distance' is Hypotenuse. (lying on the line on the 'start' and 'direction' points).
	x = sqrt(x);

	short int sign{ +1 };
	if (distance < 0) sign = -1;
	start.x += start.x < direction.x ? sign * round(x) : sign * -round(x); // is the 'start' to the left  of the 'direction'
	start.y += start.y < direction.y ? sign * round(y) : sign * -round(y); // is the 'start' higher than the 'direction'

	return { start.x, start.y };
}
//----------------------------------------------------------------------------------------

// right angle to the line
Point right_angle(Point angle_point, Point second_point, int length) // select the direction with the sign
{
	double length_line = line_length(second_point, angle_point);
	double b = abs(second_point.x - angle_point.x); // Opposite side of right triangle formed by 'angle_point' and 'second_point'
	// when cos == 0 double x gets inf, and then calculetes right
	double x = length_line / cos(b, length_line); // change to x coordinate
													// 'length_line' is Hypotinuza of triangle with right angle on the x coordinate horizontal line to the line formed by 'angle_point' and 'second_point'
													// after calculeting cos(), 'length_line' becomes Adjacent side of new triangle with right angle on the main line (formed by 'angle_point' and 'second_point') to the x coordinate horizontal line

	if (b == length_line) second_point.y += 5;  // cos = 1, angle = 0

	double xx = second_point.x;
	xx += angle_point.x < second_point.x ? -x : x;

	point_double p = point_on_line(point_double(angle_point.x, angle_point.y), // point_double for more precision at this moment
		point_double(xx, second_point.y),
		angle_point.x < second_point.x&& angle_point.y  > second_point.y ||
		angle_point.x >= second_point.x && angle_point.y < second_point.y
		? -length : length); // keep direction

	return Point(round(p.x), round(p.y));
}

//----------------------------------------------------------------------------------------

// point_double for more precision in further calculations
point_double point_on_line(point_double start, point_double direction, int distance)
// finds point on any abstract line of 2 collinear points. at 'start' 'length' == 0
// use negative 'length' for finding point on the line beyond the 'start'
{
	double a = abs(start.y - direction.y); // Opposite side of larger right triangle formed by the 'start' and 'direction' points with right angle on the start.x coordinate horizontal line
	double b = abs(start.x - direction.x); // Adjacent side of larger right triangle formed by the 'start' and 'direction' points with right angle on the start.x coordinate horizontal line
	if (a == 0) { return { start.x += start.x < direction.x ? distance : -distance, start.y }; }
	if (b == 0) { return { start.x, start.y += start.y < direction.y ? distance : -distance }; }

	short int sin90 = 1;
	double sinB = (a * sin90) / Hypotenuse(b, a); // angle of a larger right triangle. the same angle for a smaller triangle. angle formed by start.x coordinate horizontal line and line on the 'start' and 'direction' points
	double y = sinB * abs(distance); //(change to y coordinate). Opposite of smaller right triangle, where 'distance' is Hypotenuse.  (lying on the line on the 'start' and 'direction' points).
	double x = pow(distance, 2) - pow(y, 2); // (change to x coordinate). Adjacent of smaller right triangle, where 'distance' is Hypotenuse. (lying on the line on the 'start' and 'direction' points).
	x = sqrt(x);

	short int sign{ +1 };
	if (distance < 0) sign = -1;
	start.x += start.x < direction.x ? sign * x : sign * -x; // is the 'start' to the left  of the 'direction'
	start.y += start.y < direction.y ? sign * y : sign * -y; // is the 'start' higher than the 'direction'
	return { start.x, start.y };
}

// point_double for more precision in further calculations
point_double right_angle(point_double angle_point, point_double second_point, int length) // select the direction with the sign
{
	double b = abs(second_point.x - angle_point.x); // Opposite side of right triangle formed by 'angle_point' and 'second_point'
	double length_line = line_length(angle_point, second_point);
	// when cos == 0 double x gets inf, and then calculetes right
	double x = length_line / cos(b, length_line); // change to x coordinate
												// 'length_line' is Hypotinuza of triangle with right angle on the x coordinate horizontal line to the line formed by 'angle_point' and 'second_point'
												// after calculeting cos(), 'length_line' becomes Adjacent side of new triangle with right angle on the main line (formed by 'angle_point' and 'second_point') to the x coordinate horizontal line

	if (b == length_line) second_point.y += 5;  // cos = 1, angle = 0 degree.

	return point_on_line({ angle_point },
		{ second_point.x + (angle_point.x < second_point.x ? -x : x), second_point.y },
		angle_point.x < second_point.x&& angle_point.y  > second_point.y ||
		angle_point.x >= second_point.x && angle_point.y < second_point.y
		? -length : length); // keep direction
}

// point_double for more precision
point_double arrow_point2(point_double arrowpoint1, point_double linepoint, int length)
{
	return point_on_line(linepoint, arrowpoint1, -length); // take arrow point on the other side of line
}

Arrow::Arrow(Point target, Point end, int size)
	:s{ size }
{
	set_color(Color::black);

	if (s < 3) error("minimum size of arrow - 3. Selected size of arrow is", s);

	add(end); //0
	point_double linepoint{ point_on_line(point_double(target.x,target.y), point_double(end.x,end.y), s) };
	add(Point(round(linepoint.x), round(linepoint.y))); // point before arrow
	point_double arrowpoint1{ right_angle(linepoint, point_double(target.x,target.y),  s / 3) };
	Point ar1{ int(round(arrowpoint1.x)),int(round(arrowpoint1.y)) };
	add(ar1);
	add(target); //3
	point_double arrowpoint2 = arrow_point2(arrowpoint1, linepoint, s / 3);
	Point ar2{ int(round(arrowpoint2.x)),int(round(arrowpoint2.y)) };
	add(ar2);
	//add(point(1));
}
void Arrow::move_target_point(int x, int y)
{
	// calculete new arrow
	// point (0) the same. end

	set_point(3, { target_point().x + x , target_point().y + y }); // move target
	point_double linepoint{ point_on_line(point_double(target_point().x,target_point().y), point_double(end_point().x,end_point().y), s) };
	set_point(1, Point(round(linepoint.x), round(linepoint.y))); // point before arrow
	point_double arrowpoint1{ right_angle(linepoint, point_double(target_point().x,target_point().y),  s / 3) };
	Point ar1{ int(round(arrowpoint1.x)),int(round(arrowpoint1.y)) };
	set_point(2, ar1);
	set_point(3, target_point());
	point_double arrowpoint2 = arrow_point2(arrowpoint1, linepoint, s / 3);
	Point ar2{ int(round(arrowpoint2.x)),int(round(arrowpoint2.y)) };
	set_point(4, ar2);
	//	set_point(5, point(1)); // point before arrow
}
void Arrow::move_end_point(int x, int y)
{
	// calculete new arrow
   // point (1) the same

	set_point(0, { end_point().x + x , end_point().y + y }); // move end
	point_double linepoint{ point_on_line(point_double(target_point().x,target_point().y), point_double(end_point().x,end_point().y), s) };
	set_point(1, Point(round(linepoint.x), round(linepoint.y))); // point before arrow
	point_double arrowpoint1{ right_angle(linepoint, point_double(target_point().x,target_point().y),  s / 3) };
	Point ar1{ int(round(arrowpoint1.x)),int(round(arrowpoint1.y)) };
	set_point(2, ar1);
	set_point(3, target_point());
	point_double arrowpoint2 = arrow_point2(arrowpoint1, linepoint, s / 3);
	Point ar2{ int(round(arrowpoint2.x)),int(round(arrowpoint2.y)) };
	set_point(4, ar2);
	//set_point(5, point(1)); // point before arrow
}





void Arrow::draw_lines() const
{
	if (fill_color().visibility())
	{
		fl_color(fill_color().as_int());
		fl_polygon(point(2).x, point(2).y, point(3).x, point(3).y, point(4).x, point(4).y);
	}

	if (color().visibility())
	{
		fl_color(color().as_int());
		Shape::draw_lines();
		fl_line(point(4).x, point(4).y, point(1).x, point(1).y);
	}

}
































/*
test


int main()
try {


	Point p1{ 220,100  };

	Point p2{ 230,150 };

	Point pp1{ 230,110 };

	Point pp2{ 240,160 };

	Arrow ar{ p1 ,p2 ,20 };
	ar.set_color(Color::black);
	ar.set_fill_color(Color::yellow);
	window().attach(ar);

	while (true)
	{

		/*Graph_lib::Line l{ pp1, pp2 };
		l.set_color(fl_darker(fl_rgb_color(255, 0, 127)));
		window().attach(l);

		Graph_lib::Polygon p;
		//triangle_arrow(p, pp1, pp2, 10);
		p.set_color(fl_darker(fl_rgb_color(255, 0, 127)));
		p.set_fill_color(fl_darker(fl_rgb_color(255, 0, 127)));
		window().attach(p);
		*/

		//ar.move_target_point(5,0);
		/*
ar.move_end_point(-5, 0);
p1.x += 5;
pp1.x += 5;

window().wait_for_button();
	}
	window().wait_for_button();
}
catch (exception& e)
{
	cerr << "Error: " << e.what() << '.' << endl;
	keep_window_open("-1");
	return -1;
}
catch (...)
{
	cout << "Some error.\n";
	keep_window_open("-2");
	return -2;
}



*/

























/*
	// test

	int x{ 150 };
	int y{ 135 };
	int xx{ 51 };
	int yy{ 150 };
	int n = -25;
	int nn = 15;
	int nnn = 40;

	while (true) {
		Open_polyline line1;
		line1.add(Point{ x,y });
		line1.add(Point{ xx,yy });

		line1.set_color(Color::black);
		window().attach(line1);
		/
			static bool b{ 0 };
		if (b) {
			--x;
			++y;
			b = false;
		}
		else {
			++xx;
			--yy;
			b = true;
		}
		//cout << line_length(line1.point(0), line1.point(1)) << '\n';
		Graph_lib::Polygon arrow;
		triangle_arrow(arrow, line1.point(0), line1.point(1), nnn);
		// --n;
		--nn;
		--nnn;
		Point p = point_on_line(line1.point(0), line1.point(1), nn);
		Point ppp = right_angle(line1.point(1), line1.point(0), nn);
		Open_polyline line2;
		line2.add(line1.point(1));
		line2.add(ppp);
		line2.set_color(Color::red);
		window().attach(line2);

		Open_polyline line3;
		line3.add(line1.point(0));
		line3.add(p);
		line3.set_color(Color::red);
		window().attach(line3);
		
		//Graph_lib::Circle cir{p ,1 };
	//	window().attach(cir);
	//	cir.set_color(Color::red);

		



	}	window().wait_for_button();
	*/
