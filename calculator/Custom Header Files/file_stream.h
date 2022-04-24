#include "std_lib_facilities.h"


string read_file_name(istream&);

// initialise provided 'ifstream' with 'name', and return it
ifstream& initialize_ifstream(ifstream& fstream_to_initialise, string); 

// initialize 'static ofstream' with 'name', and return it
ofstream& initialize_ofstream(string name);

// for an input file
bool is_file_empty(ifstream&); 

// returns already initialized 'static ofstream' or 'cout'
ostream& fetch_ostream(); 

// returns already initialized 'static ofstream'
ofstream& get_ofstream(); 

// returns static name, for ofstream
string& ofs_name(); 

// close if open and print the close message
void close_ofstream(); 