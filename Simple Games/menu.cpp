#include "head/std_lib_facilities.h"

#include "head/3games.h"

int main()
try {
	while (true) {
		cout
			<< '\n'
			<< "1. Rock_Paper_Scissors. \n"
			<< "2. The_Game_Bull_and_Cows. \n"
			<< "3. Number guessing game. \n";
		
		char ch = 0;
		cin >> ch;
		
		
		switch (ch) {
		case '1':
			PRS();
			break;
		case '2':
			BC();
			break;
		case '3':
			NGG();
			break;
		default: 
		cout << "Please try again.\n"; 
		break;
		}
	}

keep_window_open();
return 0;
}
catch (exception& e) {
	cout << "Exception: " << e.what() << '\n';
	keep_window_open();
	return 1;
}
catch (...) {
	cout << "Something happend... " << '\n';
	keep_window_open();
	return 2;
}