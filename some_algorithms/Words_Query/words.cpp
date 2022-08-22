#include "../../../../Custom Header Files/std_lib_facilities.h"
#include <map>
using uchar = unsigned char;

class Word_Stream
{
public:
	Word_Stream(istream& istr)
		:is{ &istr }{
		_buffer.clear(ios::eofbit);
	}
	operator bool();
	Word_Stream& operator>>(string&);
	void set_replacement(string, string);
	int number_of_replacements;
private:
	map<string, string> _replacement;
	istringstream _buffer;
	istream* is;
};
void Word_Stream::set_replacement(string word, string replacement)
{
	_replacement[word] = replacement;
}
bool is_part_of_word(uchar ch)
{
	return isalpha(ch) 
		|| ch == '\''
		//|| ch == '-' 
		;
}

void replace_if_found(map<string, string>& _replacement, string& word, istringstream& _buffer,int & replacements_num )
{
	auto f = _replacement.find(word);
	if (f != _replacement.end())
	{	
		++replacements_num;
		//word = make_buffer_and_extract_word(_buffer, f->second);
		cout << _buffer.good() << ' ' << _buffer.eof() << ' ' << _buffer.fail() << " buffer1 \n";
		_buffer = istringstream{ f->second };
		cout << _buffer.good() << ' ' << _buffer.eof() << ' ' << _buffer.fail() << " buffer2 \n";
		_buffer >> word;
	}
}

string make_buffer_and_extract_word(istringstream& buffer,string replacement)
{
	istringstream is{ replacement };
	string exctracted;
	is >> exctracted;
	string buf;
	for (char ch; is.get(ch);) buf += ch;
	cout << buffer.good() << ' ' << buffer.eof() << ' ' << buffer.fail() << " buffer1 \n";
	buffer = istringstream{ buf };
	cout << buffer.good() << ' ' << buffer.eof() << ' ' << buffer.fail() << " buffer2 \n";
	return exctracted;
}
/*
vector<string> contr{ "can\'t", "ain\'t", "gimme", "cause","\'cuz","finna",
		"imma", "gonna","wanna","gotta", "hafta", "woulda", "coulda", "shoulda", "ma\'am","howdy", "let\'s","y\'all"};
	for (int n{}; n < contr.size(); ++n)
		if (contr[n] == s) return n;
	return -1;
}
string replace(const int& nn){
	vector<string> comb{ "can not", "am not", "give me","because","because",
		"fixing to","im about to", "going to", "want to", "got to", "have to","would have",
		"could have", "should have","madam","how do you do","let us" "you all" };

*/
string get_word_lowered(istream* is)
{
	char ch;
	string word;
	while (is->get(ch) && !isalpha(uchar(ch)));
	word += tolower(ch);
	while (is->get(ch) && is_part_of_word(ch))	word += tolower(ch);

	if (word.size())
		if (!isalpha(uchar(word.back()))) word.pop_back();
	return word;
}
Word_Stream& Word_Stream::operator>>(string& s)
{
	if (_buffer.good())
	{
		_buffer >> s;
		return *this;
	}
	string word{ get_word_lowered(is) };
	if (word.size())
	{
		replace_if_found(_replacement, word, _buffer,number_of_replacements);
		s = word;
	}
	return *this;
}

Word_Stream::operator bool()
{
	return is->good()|| _buffer.good();
}

void read_and_add_replacements(Word_Stream& ws, ifstream& ifs)
{

	for (char ch;  ifs >> ch && ch == '{';)
	{
		string s1, s2;
		//for (ifs >> ch; ifs && ch != ','; ifs.get(ch)) s1 += ch;
		for (ifs >> ch; ifs && ch != ','; )
		{
			s1 += ch;
			if (isspace(uchar(ch)))
				ifs >> ch;
			else
				ifs.get(ch);
		}
		if (s1.size())
			if (!isalpha(uchar(s1.back())))s1.pop_back();
		for (ifs >> ch; ifs && ch != '}'; )
		{
			s2 += ch;
			if (isspace(uchar(ch)))
				ifs >> ch;
			else
				ifs.get(ch);
		}
		if (s2.size())
			if (!isalpha(uchar(s2.back()))) s2.pop_back();
		//cout <<"{"<< s1 << ',' << s2 << "}\n";
		ws.set_replacement(s1, s2);
	}

}
/*
bool are_differ_in_s(string s1, string s2)
//assume 's' always in the second
{
	//if (s1.back() == 's') s1.pop_back();
	//else
	if (s2.back() == 's') s2.pop_back();
	return s1 == s2;
}*/
map<string, string> remove_plurals(map<string, int>& m)
{
	map<string, string> removed;
	for (auto& w : m)
	{
		if (w.first.size() > 2)
		{
			auto it = m.find(w.first+'s');
			if (it != m.end())
			{
				removed[w.first]= it->first;
				w.second += it->second;
				m.erase(it);
			}
		}
	}
	return removed;
}
template<class T,class T2>
void print_map(const map<T, T2>& words){
	char num{ 0 };
	cout << "\n----------------------------------\n";
	for (auto w : words)
	{
		cout << w.first << ":" << w.second << ".\t";
		if (num == 2)
		{
			num = 0;
			cout << '\n';
		}
		else ++num;
	}
	cout << "\n----------------------------------\n";
}

template<class T, class T2,class Pred>
void print_map_2(const map<T, T2>& words, const Pred& pred) {
	bool num{ 0 };
	cout << "\n----------------------------------\n";
	for (auto w : words)
	{
		if (pred(w))
		{
			cout << w.first << " :" << w.second << ".\t\t";
			if (num)
				cout << '\n';
			else
			if (w.first.size() < 4) cout << "\t\t\t";
			else
			if (w.first.size() < 12) cout << "\t";
			num -= 1;
		}
	}
	cout << "\n----------------------------------\n";
}
map<string, int> get_words(istream& is)
{
	Word_Stream ws{ is };
	map<string, int> words;
	ifstream ifs2{ "replacements.txt" };
	if (!ifs2)throw exception("bad replacements.txt");
	read_and_add_replacements(ws, ifs2);
	for (string w; ws >> w;)
		++words[w];
	return words;
}
/*
int main()
try
{
	string file_name{"paper.txt"};

	ifstream ifs{ file_name };
	ifstream ifs2{ "replacements.txt" };
	if (!ifs|| !ifs2) cerr << "bad file\n";
	Word_Stream ws{ ifs };
	map<string, int> words;
	read_and_add_replacements(ws, ifs2);
	
	for (string w; ws >> w;)
		++words[w];

	
	map<string, string> removed_plurals = remove_plurals(words);


	char num{ 0 };
	size_t number_of_words{ 0 };
	for (auto w : words)
	{
		cout << w.first << " :" << w.second << ".\t";
		number_of_words += w.second;
		if (num == 2)
		{
			num = 0;
			cout << '\n';
		}
		else ++num;
	}
	cout << "\n----------------------------------\n";
	cout << "\nnumber_of_replacements " << ws.number_of_replacements << '\n';
	cout << "number_of_words " << number_of_words <<'\n';
	cout << removed_plurals.size() << " removed_plurals\n";
	print_map(removed_plurals);
	keep_window_open("~");
	for (auto w : words)
	{
		cout << w.first << ",\t";

		if (num == 2)
		{
			num = 0;
			cout << '\n';
		}
		else ++num;
	}

	keep_window_open("~");


}
catch (const std::exception& e)
{
	cerr << e.what();
	keep_window_open("~");
}
*/


/*
istringstream is{ "123" };
	is >> file_name;
	cout << is.good() << ' ' << is.eof() << ' ' << is.fail() << " is \n";*/