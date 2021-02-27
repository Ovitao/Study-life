/*  
* Bulls and Cows. 
* 9 numbers. try to guess.
	
	Revisit by Tao October 2020
	Revisit by Tao September 2020

	main idea is. for simple rather boring text game. The text should be the best. (here. not the best actually)
		

	The structure is

		Main:
			intro
			play
			the_end
		play:
			make_user_case
			make_the_pack
			score
		score:
			cow_score
			bull_score
		make_user_case:
			repeated_there
		make_the_pack:
			change_repeated
		input:
			characters from '1' to '9'
		
*/
#include "head/std_lib_facilities.h"
#include "head/3games.h"

const char help = 'h';
const bool test = 0; //for testing
const string Prompt = "Enter some numbers: ";

//------------------------------------------------------------------------------------------------------------------------
void intro() 
{
	cout	<< " = Bulls and Cows =   The Game\n\n"
			<< " You have as much tries as you will need.\n"
			<< "The rules is simple: there are 4 digits from 1-9.\n"
			<< "Try to guess them. If you guess a right letter it is a 'Cow'.\n"
			<< "And if you guess the right position as well it is a 'Bull'.\n"
			<< "Which position or which letter right we do not know.\n"
			<< "And the letters must not be repeated.\n";
}
//------------------------------------------------------------------------------------------------------------------------
int cow_score (vector <char> guess_case, vector <char> the_pack) {
	int cows = 0;
	for (char p : the_pack)
		for (char g : guess_case)
			if (p == g) ++cows;
		
	return cows;
}
//------------------------------------------------------------------------------------------------------------------------
int bull_score (vector <char>& guess_case, vector <char> the_pack) {
	int bulls = 0;
	int point = 0;
	for (char g : guess_case) {							
		if (the_pack[point] == g) {  ++bulls;  guess_case[point] = 0;
		}
		++point; 
	}
	return bulls;
}
//------------------------------------------------------------------------------------------------------------------------
void score(vector <char> guess_case, vector <char> the_pack, bool& progress) {

	string bull = "bull";
	string cow = "cow";

	int bull_num = bull_score(guess_case, the_pack);
	int cows_num = cow_score (guess_case, the_pack);
	
	if (bull_num > 1) { bull += 's'; if (bull_num == 4) progress = true; };
	if (cows_num > 1) cow += 's';
	cout <<'\t' << bull_num << ' ' << bull << " and " << cows_num << ' ' << cow << '\n';
	return;
}

//------------------------------------------------------------------------------------------------------------------------

void play();
bool repeated_there(vector<char>);

bool the_end();
class  bye {};

vector<char> make_user_case();
vector<char> make_the_pack(bool& progress);
char change_repeated(char&, vector<char> );


//------------------------------------------------------------------------------------------------------------------------
void play() {
	bool win = false;
	vector <char> the_pack(make_the_pack(win)); // + the first score check and change it as well

	while (win == false){

		cout << Prompt;
		vector <char> user_case(make_user_case());
																	if (test) for (char x : the_pack) cout << " [" << x << "] "; //test
		score(user_case, the_pack, win);
	}
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int BC()
try 
{
	intro();
	while (true) {
		cout << "Enter some numbers ";

		play();

		the_end();
	}
}

catch (exception& e) {
		cout << "Exception: " << e.what() << '\n';
		return 1;
}
catch (bye) {
		return 0;
}
catch (...) {
		cout << "Unknoun exception: " << '\n';
		return 2;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------



vector<char> make_user_case() {
	while (true) {
		vector <char> user_case;
		char guess = 0;
		for (int n = 0; n != 4; ++n){  
			cin >> guess;
			while (guess == help) { intro(); cout << Prompt; cin >> guess;
			}
			user_case.push_back(guess);																			if (test) cout << '*' << guess; //test
		}
		if (repeated_there(user_case)) cerr << "\tWoaps: Repeated digit. \n"
			<< Prompt;
		else
			return user_case;
	}
}
//------------------------------------------------------------------------------------------------------------------------
vector<char> make_the_pack(bool& progress) {
	char guess = 0;
	char card = 0;
	vector <char> the_pack;
	vector <char> user_case;

	for (int n = 0; n != 4; ++n) {	
		cin >> guess; if (guess == help) { intro(); cout << Prompt; cin >> guess; } //first handle all helps 
		user_case.push_back(guess);

		//"Random". Do "randint()" many times. ( because the "crand()" doesn't work )
		for (int loop = 0; loop < abs(guess); ++loop) card = randint('1', '9');

		change_repeated(card, the_pack);
		the_pack.push_back(card);
	}

	if (repeated_there(user_case)) cerr << "\tWoaps: Repeated digit. ";
	else score(user_case, the_pack, progress);

	return  the_pack;

}

//------------------------------------------------------------------------------------------------------------------------
bool the_end() {
	cout	<< "\n\t___________________________________________\n"
			<< "\n	\tSuccess!"
			<< "\n	\tYou Won!"
			<< "\n	\tYou have discovered All the digits "
			<< "\n	\tin the Right Order! "
			<< "\n	\tCongratulations!"
			<< "\n\t___________________________________________\n";
	char start;
	cout << "Enter a character for the new game ('h' any time to see the rules . 'x' - exit). ";
	cin >> start;
	while (start == help) { intro(); cin >> start; }
	if (start == 'x') throw bye();
	else cout << "\n\tNew game!\n";
	return true;
}
//------------------------------------------------------------------------------------------------------------------------
// does the number we intend to add in =THE PULL= is a repiated number. gives a different if so
char change_repeated(char& card, vector<char>the_pack) {
	bool repeated = true;
	
	while (repeated == true) {																
		repeated = false;
		for (char x : the_pack) {																	if (test) cout << " | [ " << x << " ] , pushrand == " << card << '\n';
			if (x == card) {																if (test) cerr << " x" << card << "x\n"; //test
				repeated = true;
				card = randint('1', '9');
			}
		}
	}
	return card;
}
//------------------------------------------------------------------------------------------------------------------------
bool repeated_there(vector<char>guess_case) {
	int p2 = 0;	int p1 = 0;
	for (char d1 : guess_case) {
		++p1; p2 = 0;
		for (char d2 : guess_case) {
			++p2;
			if (d2 == d1 && p2 != p1) return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------------------

















