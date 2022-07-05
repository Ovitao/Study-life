#include "../../../../Custom Header Files/std_lib_facilities.h"
/*
here read_next_char() is called for each char from cin intil white space, and then
 new array is created with number of int max, calculated during readng. 
*/

char* create_new(int chars)
{
	char* c = new char[chars +1];
	c[chars] = 0;
	return c;
}

char* read_next_char(istream& is, int& max)
{
	char ch;
	if (is.get(ch) && !isspace(ch)) {
		char* c = read_next_char(is, ++max);
		c[max] = ch;
		--max;
		return c;
	}
	else return create_new(max--);
}
char* read_word(istream& is)
{
	char not_space;
	is >> not_space;//skiip spaces;
	if (!is) return nullptr;
	is.unget();
	int max = 0;
	return read_next_char(is, max);
}
bool is_palindrome(const char s[], int n)
{
	int first = 0;
	int last = n - 1;
	while (first < last) {
		if (s[first] != s[last]) return false;
		++first;
		--last;
	}
	return true;
}

int main()
try
{
	constexpr int max = 4;
	istream& is = cin;
	for (char* s; s = read_word(is);)
	{
		cout << s << " is";
		if (!is_palindrome(s, strlen(s))) cout << " not";
		cout << " a palindrome\n";
	}
	keep_window_open("~");
}
catch (const std::exception& e)
{
	cerr << e.what() << ".\n";
	keep_window_open("~");
}
catch (...)
{
	cerr << "Some error.\n";
	keep_window_open("~");
}