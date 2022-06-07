/*
- arrow.cpp contains functions that can calculate:
 triangle arrow at the end of a line. (uses double_point to calculate and attaches Point (int) to window) 
 coordinate of a point for perpendicular to end point of a chosen line (double or int point) (select the direction with the sign)
 coordinate of a point on any abstract line that can lye on 2 points (beyond points too). (double or int point) (select the direction with the sign)
 length between 2 points (double)
*/-------------------------------------------------------------------------------------------------------------
- any_angle_to_line.cpp contains function (angle_to_line) that allows:
to calculate point for any angle to a given line. ( with given 'degree' and 'length' for the new line.)
You can choose which side the new line will be on with sign of 'length'(inversion) or sign of 'angle' (mirror).
Tested:
	- Rotation of main line around 360 and keeping selected degree.
	- Calculating angles (-360 to 360) (and also by adding +10 degrees from -720 to 720) in 8 planes.
	- Selecting different length.
read any_angle_to_line.cpp comments for more info.
*/-------------------------------------------------------------------------------------------------------------

- Striped_closed_polyline. you can choose width, color and padding of fill lines, and get any closed polyline filled with horizontal lines

*/-------------------------------------------------------------------------------------------------------------