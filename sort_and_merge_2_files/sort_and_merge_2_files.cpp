#include "lib/std_lib_facilities.h"

void sort_file_words(const string& input_file_name, const string& output_file_name);
void sort_and_megre_2file_words(const string& input_file_name, const string& input_file_name2, const string& output_file_name);

//------------------
bool is_in_vector (const vector<int> v, const int& n)
{
	for (const int& nn : v) if (nn == n) return true;
	return false;
}
//------------------
class word
{
public:
	word()
		:name{}, number{0} {}
	string name{};
	int number{};
};
istream& operator>>(istream& is, word& w)
{
	is >> w.name;
	++w.number;
	return is;
}
//------------------
void reopen(ifstream& ifs, const string& name) // to read from the begining again
{
	ifs.clear();
	ifs.close();
	ifs.open(name);
	if (!ifs) error("reopen failure - ", name);
}
//------------------
word take_another_word(const vector<int> v, ifstream& ifs, const string & stream_name) // take word witch number not written out yet
{
	reopen(ifs, stream_name);
	for (word w; ifs >> w; ) {
		if (!is_in_vector(v, w.number)) {
			reopen(ifs, stream_name); // close, clear and open again
			return w;
		}
	}
	return word{};
}
//------------------
word take_another_word(const vector<int> v, ifstream& ifs, ifstream& ifs2)
{
	for (word w; ifs >> w || ifs2 >> w; ) {
		if (!is_in_vector(v, w.number)) {
			return w;
		}
	}
	return word{};
}
//------------------
void sort_file_words(const string& input_file_name, const string& output_file_name )
{
	ifstream ifs{ input_file_name };
	ofstream ofs{ output_file_name };
	if (!ifs) error("couldn't open input file - ", input_file_name);
	if (!ofs) error("couldn't open output file - ", output_file_name);
	ifs.exceptions(ifs.exceptions() | ios_base::badbit);
	ofs.exceptions(ofs.exceptions() | ios_base::badbit);

	//-----------------
	word smaller;
	while (ifs >> smaller); // count words
	int words_left{ smaller.number };
	reopen(ifs, input_file_name); // close, clear and open again
	vector<int> stored_word_numbers{}; // word numbers that is written out
	word current_w;
	//-----------------
	while (words_left != 0)
	{
		while (ifs >> current_w)
			if (smaller.name > current_w.name
				&& !is_in_vector(stored_word_numbers, current_w.number)) smaller = current_w;


		if (ifs.eof()) {
			stored_word_numbers.push_back(smaller.number);
			ofs << smaller.name << ' ';
			--words_left;
			current_w = {};
			smaller = take_another_word(stored_word_numbers, ifs, input_file_name); // take word that is not written out yet
		}
		else if (!ifs) error("error reading: ", input_file_name);
	}
}
//------------------
void sort_and_megre_2file_words(const string& input_file_name, const string& input_file_name2, const string& output_file_name)
{
	ifstream ifs{ input_file_name };
	ifstream ifs2{ input_file_name2 };
	ofstream ofs{ output_file_name };
	if (!ifs) error("couldn't open input file - ", input_file_name);
	if (!ofs) error("couldn't open output file - ", output_file_name);
	ifs.exceptions(ifs.exceptions() | ios_base::badbit);
	ofs.exceptions(ofs.exceptions() | ios_base::badbit);

	//-----------------
	word smaller;
	while (ifs >> smaller || ifs2 >> smaller); // count words
	int words_left{ smaller.number };
	reopen(ifs, input_file_name); // close, clear and open again
	vector<int> stored_word_numbers{}; // word numbers that is written out
	word current_w;
	//-----------------
	while (words_left != 0)
	{
		while (ifs >> current_w || ifs2 >> current_w)
			if (smaller.name > current_w.name
				&& !is_in_vector(stored_word_numbers, current_w.number)) smaller = current_w;


		if (ifs.eof()) { // done searching
			stored_word_numbers.push_back(smaller.number);
			ofs << smaller.name << ' ';
			--words_left;
			current_w = {};
			reopen(ifs, input_file_name); //  clear, close and open again
			reopen(ifs2, input_file_name2);
			smaller = take_another_word(stored_word_numbers, ifs, ifs2); // take word that is not written out yet
			reopen(ifs, input_file_name); 
			reopen(ifs2, input_file_name2);
		}
		else if (!ifs) error("error reading: ", input_file_name);
	}
}
//------------------

int main()
try {
	string name1{ "first.txt" };
	string name2{ "second.txt" };
	string name3{ "sorted.txt" };

	sort_and_megre_2file_words(name1, name2, name3);
	
	//-----------------
	{
		cout << " Success!!!\n";

		keep_window_open("1");


	}
}
catch (exception& e)
{
	cout << e.what() << endl;

	keep_window_open("-1");

}
catch (...)
{
	cout << "some error" << endl;

	keep_window_open("~2");

}
