#include "../../../Custom Header Files/Graphical_Headers/Simple_window.h"
// 6.1.22

class Striped_Closed_polyline:public Closed_polyline
{
public:
	//using  Closed_polyline::public Closed_polyline;
	Striped_Closed_polyline() :Closed_polyline() {}
	Striped_Closed_polyline(initializer_list<Point> lst) :Closed_polyline{ lst } {}

	void set_padding(int);
	const int& padding() const;

	void set_fill_width(int);
	const int& fill_width() const;

	void draw_lines() const;
private:
	int pad{ 2 };
	int w{ 2 };
};

void Striped_Closed_polyline::set_padding(int p)
{
	if (p < 1) error("padding is less than 1");
	pad = p;
}
const int& Striped_Closed_polyline::padding() const
{
	return pad;
}
void Striped_Closed_polyline::set_fill_width(int w)
{
	if (w < 1) error("fill width is less than 1");
	Striped_Closed_polyline::w = w;
}
const int& Striped_Closed_polyline::fill_width() const
{
	return w;
}


void Striped_Closed_polyline::draw_lines() const
{
	if (fill_color().visibility())
	{
		fl_color(fill_color().as_int());
		fl_line_style(style().style(), w);
		short int y_min = point(0).y;
		short int y_max = y_min;
		constexpr short int x_min = -32768;
		constexpr short unsigned int x_max = -1;
		const short int padding = pad + w;
		for (int i = 1; i < number_of_points(); ++i)
		{
			if (point(i).y < y_min)
				y_min = point(i).y;
			if (point(i).y > y_max)
				y_max = point(i).y;
		}
		Point p = { 0,0 };
		vector <short int> intersections_x;
		for (y_min += w / 2; y_min < y_max; y_min += padding)
		{
			intersections_x = {}; // start with empty
			for (short int i = 1; i <= number_of_points(); ++i)
				if (line_segment_intersect(point(i == number_of_points() ? 0 : i), point(i - 1), Point(x_min, y_min), Point(x_max, y_min), p))
					intersections_x.push_back(p.x);
			sort(intersections_x);
			for (short int n = 1; n < intersections_x.size(); n += 2)
				fl_line(intersections_x[n - 1], y_min, intersections_x[n], y_min);
		}
		fl_color(color().as_int());
		fl_line_style(style().style(), style().width());
	}
	if (color().visibility())
	{
		fl_color(color().as_int());
		Shape::draw_lines();
		fl_line(point(0).x, point(0).y, point(number_of_points() - 1).x, point(number_of_points() - 1).y);
	}
}





Simple_window& win()
{
	static	Simple_window win{ { 100,500 },900,600,"Striped_polyline" };
	return win;
}


int main()
try
{
	
	Marked_polyline mk{ "0123456789" };
	mk.add({ 600,100 });
	mk.add({ 600,200 });
	mk.add({ 400, 150 });
	mk.add({ 200, 100 });
	mk.add({ 200,200 });
	mk.set_color(Color::black);
	//win().attach(mk);

	//Striped_Closed_polyline src{ { 600,100 },{ 600,200 }, {200, 100}, { 200,200 }};
	//Striped_Closed_polyline src{ { 100,100 },{ 200,100 }, {300, 100}, { 400,100 } };

/*
	Striped_Closed_polyline src{
	{400, 100}, { 800,500 }, { 500,500 }
};
	Striped_Closed_polyline src{
	{400, 100}, { 800,100 }, { 500,500 },
};
	*/

	/*Striped_Closed_polyline src{
		{400, 100}, { 800,200 }, { 800,100 }, { 400,200 }, { 500,200 }
		Striped_Closed_polyline src{
			  {400,250} ,{410,40} ,{500,900}, {420,10},
			 {400,10}, {290,130} , {210,90} , {210,190}
		};
		src.add({ 200, 300 });
		src.add({ 190, 280 });
		src.add({ 260, 200 });
		src.add({ 280, 240 });
		src.add({ 360, 190 });
		src.add({ 380, 300 });
		src.add({ 270, 280 });
		src.add({ 250, 250 });

		mk.add({ 200, 300 });
		mk.add({ 190, 280 });
		mk.add({ 260, 200 });
		mk.add({ 280, 240 });
		mk.add({ 360, 190 });
		mk.add({ 380, 240 });
		mk.add({ 270, 280 });
		mk.add({ 250, 250 });*/

	Striped_Closed_polyline src;
	src.add({ 200, 300 });
	src.add({ 190, 280 });
	src.add({ 260, 200 });
	src.add({ 280, 240 });
	src.add({ 360, 190 });
	src.add({ 380, 240 });
	src.add({ 270, 280 });
	src.add({ 250, 250 });

	src.set_padding(3);
	src.set_fill_width(1);
	
	cout << src.padding() << '\n';
	cout << src.fill_width() << '\n';

	src.set_color(Color::black);
	src.set_fill_color(Color::magenta);


	win().attach(src);



	//
	/*
	Striped_Closed_polyline src{
		  {400,10} ,{410,40} ,{500,900}, {420,10}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};
	Striped_Closed_polyline src{
		  {400,10} ,{410,40} ,{500,900}, {420,10}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};
	Striped_Closed_polyline src{
		  {400,10} ,{410,40} ,{500,900}, {420,10}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};
		Striped_Closed_polylinesrc{
		  {400,10}, {420,10} ,{500,900} ,{410,40}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};

		Striped_Closed_polyline src{
	  {400,10} ,{410,40}, {420,10} ,{500,900}
	,{400,250}, {290,130} , {210,90} ,{210,190}
	};


	Striped_Closed_polyline src{
		  {400,10}, {420,10} ,{500,900} ,{410,40}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};

	Marks mk{ "01234567" };
	mk.add({ 400,250 });
	mk.add({ 410,40 });
	mk.add({ 500,900 });
	mk.add({ 420,10 });
	mk.add({ 400,10 });
	mk.add({ 290,130 });
	mk.add({ 210,90 });
	mk.add({ 210,190 });
	Marked_polyline mrks{ ".,'" };
	mrks.add({ 0,0 });
	mrks.add({ 4,4 });
	mrks.add({ 1,2 });
	mrks.set_color(Color::black);
	win().attach(mrks);

	Striped_Closed_polyline src{
		  {400,10} ,{410,40}, {420,10} ,{500,900}
		,{400,250}, {290,130} , {210,90} ,{210,190}
	};
	src.add({ 200, 300 });
	src.add({ 190, 280 });
	src.add({ 260, 200 });
	src.add({ 280, 260 });
	src.add({ 360, 190 });
	src.add({ 380, 240 });
	src.add({ 270, 280 });
	src.add({ 250, 250 });


	*/



	Closed_polyline cp;
	cp.add({ 200, 300 });
	cp.add({ 190, 280 });
	cp.add({ 260, 200 });
	cp.add({ 280, 240 });
	cp.add({ 360, 190 });
	cp.add({ 380, 240 });
	cp.add({ 270, 280 });
	cp.add({ 250, 250 });
	//cp.add({ 320, 200 });
	cp.set_color(Color::black);
	cp.set_fill_color(Color::magenta);
	win().attach(cp);
	cp.move(0, 100);

	/*
	double b = 10;
	while (true)
	{
		Right_triangle src{ {200,200}, 100,b };
		src.set_color(Color::black);
		src.set_fill_color(Color::magenta);
		window().attach(src);
		//cout << "src.angle_a2();  " << src.angle_a2() << " src.angle_b2();  " << src.angle_b2() << '\n';
		cout << "src.angle_a();  " << src.angle_a() << " src.angle_b();  " << src.angle_b() << '\n';

		cout << b << '\n';

		++b;
		window().wait_for_button();
	}
	*/

	win().wait_for_button();

}
catch (exception& e)
{
	cout << "Error: " << e.what() << '.' << endl;
	keep_window_open("~");
}



