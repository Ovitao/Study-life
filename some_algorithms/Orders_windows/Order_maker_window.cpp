#include "Order_Maker_Window.h"

void Order_Maker_Window::switch_()
{
	if (new_order.active())
	{
		customer_name.activate();
		address.activate();
		data.activate();
		make_order.activate();

		new_order.deactivate();
		add_purchase.deactivate();
		product_name.deactivate();
		unit_price.deactivate();
		count.deactivate();
	}
	else
	{
		customer_name.deactivate();
		address.deactivate();
		data.deactivate();
		make_order.deactivate();

		new_order.activate();
		add_purchase.activate();
		product_name.activate();
		unit_price.activate();
		count.activate();;
	}
	purchases_added = false;
}

void Order_Maker_Window::open_file_()
{
	ofs.close();
	ofs.open(file_name.value(), ios::app);
	redraw();
}

void Order_Maker_Window::make_order_()
{
	current_order =
	{ customer_name.value(),
		 address.value(),
		 data.value() };
	if (current_order.name.size() || current_order.address.size() || current_order.data.size())	switch_();

}

void Order_Maker_Window::new_order_()
{
	if (ofs.is_open())
	{
		ofs << current_order << endl;
		switch_();
	}
	else
		ofs.clear(ios::failbit);
	redraw();
}
void Order_Maker_Window::add_purchase_()
{
	istringstream is{ string(unit_price.value()) + ' ' + count.value() };
	double price;
	short count;
	purchases_added = bool(is >> price >> count);
	if (purchases_added)
		current_order.purchases.push_back(Purchase{ product_name.value(),price,count });
	else
		purchases_added = -1;
	//cout << "purchases_added " << bool(purchases_added )<< '\n';
	redraw();
}

Order_Maker_Window::Order_Maker_Window()
	:Window{ 600,800,"Order_Maker" },
	file_name{ 120,10,120,20,"File name" },
	open_file{ file_name.x(),file_name.y() + file_name.h() + 10,80,file_name.h() ,"Open file" },

	customer_name{ file_name.x() ,open_file.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Customer name" },
	address{ file_name.x() ,customer_name.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Address" },
	data{ file_name.x() ,address.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Data" },
	make_order{ file_name.x(), data.y() + file_name.h() + 10, open_file.w(), open_file.h(),"Make order" },
	new_order{ make_order.x() + make_order.w() + 10 , make_order.y(), make_order.w(), make_order.h(),"New order" },

	product_name{ file_name.x() ,make_order.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Product name" },
	unit_price{ file_name.x() ,product_name.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Unit price" },
	count{ file_name.x() ,unit_price.y() + file_name.h() + 10,file_name.w(),file_name.h(),"Count" },
	add_purchase{ file_name.x(), count.y() + file_name.h() + 10, open_file.w() + 15, open_file.h(),"Add purchase" },
	exit{ x_max() - open_file.w() - 10,file_name.y(),open_file.w(), open_file.h(),"Exit" }
{
	add(file_name);	add(open_file);
	add(customer_name);	add(address);
	add(data);	add(make_order);	add(new_order);
	add(product_name);	add(unit_price);
	add(count);	add(add_purchase); add(exit);

	new_order.callback([](Fl_Widget*, void* v) { reference_to<Order_Maker_Window>(v).new_order_(); }, this);
	open_file.callback([](Fl_Widget*, void* v) { reference_to<Order_Maker_Window>(v).open_file_(); }, this);
	make_order.callback([](Fl_Widget*, void* v) { reference_to<Order_Maker_Window>(v).make_order_(); }, this);
	add_purchase.callback([](Fl_Widget*, void* v) { reference_to<Order_Maker_Window>(v).add_purchase_(); }, this);
	exit.callback([](Fl_Widget*, void* v) {
		if (reference_to<Order_Maker_Window>(v).new_order.active())
			reference_to<Order_Maker_Window>(v).new_order_();
		reference_to<Order_Maker_Window>(v).hide();
		}, this);

	switch_();

	purchases_added = false;

	end();
}
void Order_Maker_Window::draw()
{
	if (visible())
	{
		Window::draw();
		if (ofs.fail())
		{
			fl_color(Color::red);
			fl_line_style(Line_style::solid, 3);
			fl_rect(file_name.x(), file_name.y(), file_name.w(), file_name.h());
		}
		else
			if (ofs.is_open())
				fl_draw("File is open.", file_name.x() + file_name.w() + 10, file_name.y() + file_name.h() / 2);

		if (purchases_added)
		{
			string aded;
			if (purchases_added == -1)	aded = "Not ";
			aded += "Added.";
			fl_draw(aded.c_str(), add_purchase.x() + add_purchase.w() + 10, add_purchase.y() + add_purchase.h() / 1.5);
		}
	}
}