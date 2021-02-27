


/*
The game case:
................................................................................................
"=Rock & Paper & Scissors=" 
1."Rock"
2. "Paper"
3. "Scissors" 
 "Your choose: "   

-----/compare with PC hand/-----

"You win/You loose/ It's even. "
................................................................................................
*/ 

#include "head/std_lib_facilities.h"
#include "head/3games.h"

int PRS() {
	std::cout << '\t'<< "=Rock & Paper & Scissors=" << '\n'; 
	std::cout << '\n';
	vector <int> pc = { 0, 1, 2 };
	int user_choose = 1;
	int pc_choose = 0;
	int o = 0;
	int l = 0;
	vector <string> pool = { "Rock", "Paper", "Scissors" };
	for (int k = 1; k == 1; ) {
		
		switch (l) {
		case 0:
			o = pc[0];
			pc[0] = pc[1];
			pc[1] = o;
			o = pc[1];
			pc[1] = pc[2];
			pc[2] = o;
			l = 1;
			break;
		case 1:
			o = pc[0];
			pc[0] = pc[2];
			pc[2] = o;
			l = 0;
			break;

		}
		
		//for (int y : pc) {std::cout << ' ' << '[' << y << ']';} //answers
		int result = 2;
		std::cout << "\n1. " << pool[0] << '\n'
			  << "2. " << pool[1] << '\n'
			 << "3. " << pool[2] << '\n'
			<< "q" <<  " - quit \n"
			<< "Your choose: ";
		char c= 0;
		
		while(c < '1' || c>'3') {
		std::cin >> c;
		if(c=='q') return 0;
		}
		user_choose=c-'0';
		
		user_choose -= 1;
		pc_choose = pc[user_choose];
		string user_hand = pool[user_choose];
		string pc_hand = pool[pc_choose];
		std::cout << '\n' << '\t' << user_hand << " against " << pc_hand << '\n';
		string next = "???";
		string past = "???";
		while (result == 2) {
			for (string x: pool) {
				past = next;
				next = x;
				if (pc_hand == user_hand) {
					result = 3; //even
				}
				else if (user_hand == past && pc_hand == next)
				{
					result = 1; //pc win		
				}
				else if (user_hand == next && pc_hand == past)
				{
					result = 0; //user win
				}  
			}
		}
		switch (result) {
		case 0:
			cout << "You win." << '\n' << '\n';
			break;
		case 1:
			cout << "You loose. " << '\n' << '\n';
			break;
		case 3:
			cout << "It's even.  " << '\n' << '\n';
			break;
		}
		
	}

	return 0;
}