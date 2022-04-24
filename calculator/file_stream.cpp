#include "Custom Header Files/file_stream.h"
#include "Custom Header Files/Other.h"
#include "constants.cpp"

//---------------------------------------------------------------------
string& ofs_name()
{
	static string name;
	return name;
}
//---------------------------------------------------------------------
string read_file_name(istream& in)
{
	string name;
	bool is_file{ false };
	char c;
	while (in.get(c) && is_space(c) && c != print); // pass spaces

	while (c != print) {
		name += c;
		if (c == '.') { 
			in.get(c); 
			if (!is_letter(c)) break; // check the next after the dot
			name += c;
			is_file = true;
			break;
		}
		in.get(c);
	}
	if (is_file) {
		for (char c; in.get(c) && !is_space(c); name += c);
		if (!in.eof())
			in.unget();
		return name;
	}
	if (!in.eof()) in.unget();
	error("file name expected e.g. text.txt");
}
//---------------------------------------------------------------------
// initialize 'static ofstream' with 'name', and return it
ofstream& initialize_ofstream(string name)
{
	static ofstream ofs;
	if (name.empty()) return ofs; // get already created
 	ofs.close(); //to open a new file
	ofs.open(name);
	if (ofs.fail()) error("couldn't open output file - ", name);

	ofs.exceptions(ofs.exceptions() | ios_base::badbit);
	return ofs;
}
//---------------------------------------------------------------------
// returns already initialized 'static ofstream' or 'cout'
ostream& fetch_ostream()
{
	if (get_ofstream().is_open()) 
		return get_ofstream();
	else return cout;
}
//---------------------------------------------------------------------
// returns already initialized 'static ofstream'
ofstream& get_ofstream()
{
	return initialize_ofstream(string());
}
//---------------------------------------------------------------------
void close_ofstream()
{
	if (get_ofstream().is_open()) {
		get_ofstream().close(); 
		cout <<  "Closed output file " << ofs_name() << ". Writing finished." << endl;

	}
}
//---------------------------------------------------------------------
// for an input file
bool is_file_empty (ifstream& ifs) 
{
	char c;
	if (!(ifs >> c)) return true;
	ifs.unget();
	return false;
}
//---------------------------------------------------------------------
// initialise provided 'ifstream' with 'name', and return it
ifstream& initialize_ifstream(ifstream& ifs, string name)
{
	ifs.close(); // to open a new file
	ifs.clear(); // clear from the "eof" status
	ifs.open(name);
	if (ifs.fail()) error("couldn't open input file - ", name);

	if (is_file_empty(ifs)) error(name, " is blank");

	ifs.exceptions(ifs.exceptions() | ios_base::badbit);
	//ifs.exceptions(ifs.exceptions() | ios_base::eofbit);

	return ifs;
}
//---------------------------------------------------------------------