#include "Quering_Orders_Window.h"
#include "Order_Maker_Window.h"

int main()
try
{

	Order_Maker_Window omw;

	Querying_Orders_Window qow;

	Fl::run();
}
catch (const std::exception& e)
{
	cerr << e.what();
	keep_window_open("~");
}