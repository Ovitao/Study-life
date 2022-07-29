#include "Order.h"
/*
it adds order records to a file. if file is not open it will indicate this with  a red rectangle around text field.
Before the order is created with "Make order" button(at least one field must be not empty), "Add Purchase" section is inactive, 
after the order is created, Make section is inactive until all purchases is added and "New Order" (or "Exit" just writes current order and exits) button is pressed.
*/

class Order_Maker_Window : public Window
{
public:
	//	using Window::Window;
	Order_Maker_Window();
private:
	Fl_Input file_name;
	Fl_Button open_file;

	Fl_Input customer_name;
	Fl_Input address;
	Fl_Input data;
	Fl_Button make_order;
	Fl_Button new_order;

	Fl_Input product_name;
	Fl_Input unit_price;
	Fl_Input count;
	Fl_Button add_purchase;
	Fl_Button exit;

	ofstream ofs;

	void open_file_();
	void make_order_();
	void add_purchase_();
	void new_order_();
	void switch_();
	Order current_order;

	char purchases_added;

	void draw();
};




//if (ofs.is_open())
	//redraw();
//"Out put file"

//cout << children() << '\n';	cout << x_max() << 'm' << y_max() << '\n';
