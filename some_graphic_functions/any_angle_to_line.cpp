// 5.11.22
/*
The file contains function (angle_to_line) to calculate any angle to a given line. ( with given 'degree' and 'length' for the new line.)
You can choose which side the new line will be on with sign of 'length'(inversion) or sign of 'angle' (to mirror).
Tested:
	- Rotation of main line around 360 and keeping selected degree.
	- Calculating angles (-360 to 360) (and also by adding +10 degrees from -720 to 720) in 8 planes,
							where angle point 'A' relates to the end of the main line 'E' as: 
																1. A.x < E.x && A.y < E.y
																2. A.x < E.x && A.y = E.y
																3. A.x < E.x && A.y > E.y
																4. A.x = E.x && A.y > E.y
																5. A.x > E.x && A.y < E.y
																6. A.x > E.x && A.y = E.y
																7. A.x > E.x && A.y > E.y
																8. A.x = E.x && A.y > E.y
	- selecting different length.
																							
*/
#include "angles.h"

double pi()
{
	return 3.141592653589793238;
}
double as_degree(double x)
// radian to degree
{
	return round((x*180)/pi());
}
double as_radian(double x)
// degree to radian 
{
	return (x * pi() ) / 180;
}

Point try_vertical_line(Point , Point , int , int , double );
Point try_horizontal_line(Point, Point, int, int);

Point angle_to_line(Point angle_point, Point p2, int C1_angle, int length)
// 'new_line' is line formed with C1_angle (to the main line: angle_point and p2 line) and extended to p2.x coordinate 
{
	if (C1_angle >= 360 || -360 >= C1_angle) C1_angle %= 360; // throw away circles 
	if (C1_angle > 180 || -180 > C1_angle) { length *= -1; C1_angle %= 180; } // reflex angle. mirror to the other side of the main line. angles that > 180

	if (C1_angle < 0) {
		length *= -1;
		C1_angle = 180 - abs(C1_angle);
	}

	if ((angle_point.x <= p2.x && angle_point.y < p2.y) // cope with rotation and keeping place
		|| (angle_point.x > p2.x && angle_point.y >= p2.y))
	{length *= -1; C1_angle = 180 - abs(C1_angle);}// mirror angles that < 180

	return try_horizontal_line(angle_point, p2, C1_angle, length);
}
//-------------------------------------------------------------------------------------------------

Point try_horizontal_line(Point angle_point, Point p2, int C1_angle, int length)
// find reference point on horizontal line at p2.y (reference line)
/*
	calculates 1st triangle and then one of 2 other triangles to get point on reference line that makes C1_angle;
	1st is formed by main line (angle_point and p2), reference line, and 'y_side' (angle_point.y - p2.y);
	2d is formed by 'y_side', 'length line'(new requiered line) extended to the reference line, and part of the reference line that is 'x_change'.
	3d is formed by 'y_side', 'length line' and horizontal line at angle_point.y that is 'x_change'.
*/
{
	double y_side = abs(angle_point.y - p2.y);
	double sinY = y_side / line_length(angle_point, p2); // sinY angle (p2) of main line and the reference line. (law of sine)
	double Y_angle = as_degree(asin(sinY));
	double B_angle = 180 - (Y_angle + C1_angle); //degree. angle of extended 'length' line and the reference line 

	//----------------------------------------------------------------------------
	if ((B_angle < 0 && abs(B_angle) !=90) || Y_angle == 0) return try_vertical_line(angle_point, p2, C1_angle, length, Y_angle);
	//----------------------------------------------------------------------------

	double B2_angle = (B_angle < 90 ? 90 : 180) - B_angle; // degree. angle of the 'length line' and the 'x_change' line (part of the reference line from the other side of B_angle) (2d triangle). if B_angle < 90 then B2_angle is angle of the 'length line' and the 'y_side' (3d triangle)
	double C2_angle = 180 - 90 - B2_angle;// degree. angle of 'y_side' and 'length line'(2d triangle). if B_angle < 90 then C2_angle is angle of the 'length line' and 'x_change' line (3d triangle).
	
	double x_change{ 0 }; // change (x coordinate). by law of sine
	if (B_angle < 90)
	{
		x_change -= (sin(as_radian(B2_angle)) * y_side) / sin(as_radian(C2_angle)); // 2d triangle
		if (B_angle == -90) length *= -1; // vertical line. C1_angle angle 0
	}
	else  x_change += (sin(as_radian(C2_angle)) * y_side) / sin(as_radian(B2_angle)); // 3d triangle

	point_double p = point_on_line(point_double(angle_point.x, angle_point.y), // get selected length
		point_double(angle_point.x + (angle_point.x <= p2.x ? x_change : -x_change), p2.y), length); // is angle_point to the left from p2
	return Point(round(p.x), round(p.y));
}
Point try_vertical_line(Point angle_point, Point p2, int C1_angle, int length, double Y_angle)
// find reference point on vertical line line at new genegated p2.y (reference line)
/*
	draws auxiliary line along beyond angle_point to the new p2 
	calculates 2 triangles witch is the auxiliary line + 'x_side'(horizontal line) + 'reference line'
	and 2d is 'length line' ( required line extended to 'reference line') + 'y_change' (vertical line) + 'x_side'
*/
{
	if (Y_angle != 0) p2 = point_on_line(angle_point, p2, -100); // make auxiliary line beyond angle_point, if main line is not horizontal
	if (Y_angle == 90) p2.x = angle_point.x - 100; //  move auxiliary line symmetrically. main line is vertical
	
	double A_angle = 180 - Y_angle; 
	A_angle = C1_angle - A_angle; // additional angle beyond horizontal line at the angle_point.y coordinate
	double sinA = sin(as_radian(A_angle));
	double x_side = abs(angle_point.x - p2.x);
	double sinX = sin(as_radian(180 - A_angle - 90)); // angle which Opposite is 'x_side'
	double y_change = (sinA * x_side) / sinX;

	if (y_change < 0) length *= -1; // cope with direction;
	if (y_change == 0 && A_angle == 0) length *= -1; // cope with direction. horizontal line. C1_angle 180 

	point_double p = point_on_line(point_double(angle_point.x, angle_point.y),// get selected length
		point_double(p2.x, angle_point.y + (angle_point.y < p2.y ? y_change : -y_change)),	length); // is the angle_point above the p2
	return Point(round(p.x), round(p.y));
}






















/* test
int main()
try {
	vector<Point> pv1{ Point{80,120}, Point{50,110},Point{ 93,107 },Point{105,95},
						Point{ 130,100 }, Point{120,110},Point{116,114}, Point{105,125}
		       };
	vector<Point> pv2{ Point{ 130,100 }, Point{500,110}, Point{ 116,114 },	Point{105,300},
						Point{ 80,120 },Point{90,110},Point{94,106}, Point{105,95}
		
		};
	
	int x_end{ 80 };
	int y_end{ 120 };
	int xx_target{ 130 };
	int yy_target{ 100 };
	
	int angle = 0;
	int lenght = -30;
	

	for (int n{ 0 };n < pv1.size();	n+=1 ) 
	{
		angle = 370;
	
		while(angle > -370)
	{
		angle -= 10;
		//n = 3;

		if (n > pv1.size() - 1) n = 0;

		Open_polyline line1;
		line1.add(pv2[n]);
		//line1.add(Point{ xx_target,yy_target });
		line1.add(pv1[n]);
		//line1.add(Point{ x_end,y_end });

		//--lenght;


		/*
		static bool x_side{ 0 };
		if (x_side) {
			++x_end;
			--y_end;
			x_side = false;
		}
		else {
			--xx_target;
			++yy_target;
			x_side = true;
		}
		*/

/*
		line1.add(point_on_line(line1.point(1), line1.point(0), -5));
		line1.set_color(Color::black);
		
		window().attach(line1);
	
		Point pp = angle_to_line(line1.point(1), line1.point(0), angle, lenght);
		Open_polyline line2;
		line2.add(line1.point(1));
		line2.add(pp);

		line2.set_color(Color::red);
		window().attach(line2);

		Open_polyline line3;
		Point p = right_angle(line1.point(1), line1.point(0), -15);
		line3.add(line1.point(1));
		line3.add(p);
		line3.set_color(Color::green);
		window().attach(line3);

	

		Graph_lib::Circle cir{ line1.point(0) ,3 };
		window().attach(cir);
		cir.set_color(Color::red);
		cir.set_fill_color(Color::red);



		window().wait_for_button();
	}
}
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

