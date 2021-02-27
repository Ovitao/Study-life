// ask questions about number you imagine

#include "head/std_lib_facilities.h"
#include "head/3games.h"

int NGG() {
	std::cout << "=Guessing a number [1:100] in 7 or less questions=";
	std::cout << '\n';

	//=test= for (int c = 1; c != 101; ++c) {
	
	string ans = "???" ;
	int min = 1;
	int max = 101;
	int q = 50;
	int pr = 1;
	int aft = 0;
		for (int i = 1; i != 8; ++i) {
			if (pr != aft) { //check prevents 1 extra question
				std::cout << '\n';
				std::cout << i << ". Is the number you are thinking of less than " << q << ':' << ' ';
				cin >> ans;
				//=test= if (c < q) ans = "yes"; else  ans = "no";

				pr = q;
				if (ans == "no") // if a guessing number greater
				{
					min = q;  //sets an edge on the min
					q = min + (max - min) / 2; // takes the midpoint between min and max
					aft = q;

				}
				else if (ans == "yes") // if a guessing number is less
				{
					max = q;  //sets the boundary at max
					q = max - (max - min) / 2; // takes the midpoint between min and max
					aft = q;
				}
				else {
					std::cout << '\t' << "Try yes or no."; 
					--i; 
				}
			}

		}
			std::cout << '\n';
			std::cout << '\t' << "It is " << min << "!" << '\n';;
			std::cout << '\n';
		
		//=test= if (min != c) simple_error(" not that number.'\n'"); }
	
	
	return 0;
}