#include "Header.h"
#include <Windows.h>
#include <chrono>
#include <ctime>
//------------------------------------------------------------------------------
double Hypotenuse(double a, double b) {
	double c = pow(a, 2) + pow(b, 2);
	c = sqrt(c);
	return c;
}

double line_length(Point first, Point end)
{
	double a = abs(first.y - end.y);
	double b = abs(first.x - end.x);
	if (a == 0) return b; // length == x1-x2 coordinate, horizontal line
	if (b == 0) return a; // length == y1-y2 coordinate, vertical line

	return Hypotenuse(a, b);
}

bool is_rus(wchar_t ch)
{
	return (ch >= L'а'&& L'я' >= ch) 
	|| 	(ch >= L'А' && 	L'Я' >= ch);
}
/*
bool is_eng(wchar_t ch)
{
	return (ch >= L'a' && L'z' >= ch)
		|| (ch >= L'A' && L'Z' >= ch);
}*/
//------------------------------------------------------------------------------

wistream& operator>>(wistream & is, Point & p)
{
	double x, y;
	if (is >> x >> y) {
		p.x = x;
		p.y = y;
	}
	//else  if (!is.eof())		is.clear(ios_base::failbit);

	return is;
}
wistream& operator>>(wistream & is, Object & o)
{
	wstring name;
	Point point;
	wstring type;
	double time;
	if (is >> name >> point >> type >> time) {
		o.name = name;
		o.point = point;
		o.type = type;
		o.time = time;
	}
//	else if (!is.eof())	is.clear(ios_base::failbit);
	
	return is;
}
wistream& operator>>(wistream& is, Group& pg)
{
	Group p;
	while (is && !is.eof()) {
		Object o;
		is >> o;
		p.push_back(o);
	}

	if (is.eof()) pg = p; 
	//else is.clear(ios_base::failbit);

	return is;
}
wostream& operator<<(wostream& os, const Point& p)
{
	return os << p.x << ' ' << p.y;
}
wostream& operator<<(wostream& os,const Object& p)
{
	return os << p.name << ' ' << p.point << ' ' << p.type << ' ' << p.time;
}
wostream& operator<<(wostream& os, const Group& p)
{
	for (const auto& a : p)	os << a << endl;
	return os;
}

//------------------------------------------------------------------------------

void insert_in_Name_Group(Name_Group& dg, Object* ob)
{
	if(is_rus(ob->name[0])) dg[ob->name[0]].insert( { ob->name, ob });
	else dg[L'#'].insert({ob->name, ob});
}
bool fill_Name_Group(Name_Group& dg, Group& ob)
{
	if (is_created_Name_Group(dg)) return false;
	
	for (auto& a : ob)
		insert_in_Name_Group(dg, &a);
	return true;
}
wostream& operator<<(wostream& os, const Name_Group& p)
{
	for (const auto& a : p)
	{
		os << '{'<<a.first <<'}' << endl;
		for (const auto& at : a.second)
			os << *at.second << endl;
	}
	return os;
}

//------------------------------------------------------------------------------

void insert_in_Type_Group(Type_Group& dg, Object* ob)
// if at least two objects of the type exist - create a group
{
	if (find_if(dg.begin(), dg.end(),
		[ob](const auto& vt) {return vt.first == ob->type; }) != dg.end())
		dg[ob->type].insert({ ob->name, ob });
	else {
		auto a = find_if(dg[L"Разное"].begin(), dg[L"Разное"].end(),
			[ob](const multimap<wstring, Object*>::value_type& vt) {return vt.second->type == ob->type; });
		if (a != dg[L"Разное"].end())
		{
			dg[ob->type].insert({ ob->name, ob });
			dg[ob->type].insert({ (*a).second->name, ob });
			dg[L"Разное"].erase(a);
		}
		else dg[L"Разное"].insert({ ob->name, ob });
	}
}
bool fill_Type_Group(Type_Group& dg, Group& ob)
{
	if (is_created_Type_Group(dg)) return false;

	dg[L"Разное"]; //create
	for (auto& a : ob)
		insert_in_Type_Group(dg, &a);
	return true;
}
wostream& operator<<(wostream& os, const Type_Group& p)
{
	for (const auto& a : p)
	{
		os << '{' << a.first <<'}' << endl;
		for (const auto& at : a.second)
			os << *at.second << endl;
	}
	return os;
}
//------------------------------------------------------------------------------
void insert_in_Distance_Group(Distance_Group& dg, Object* ob)
{
	double d = line_length({ 0,0 }, ob->point);
	if (d < 100)
		dg.below_100.insert({ d, ob });
	else
	if (d < 1000)
		dg.below_1000.insert({ d, ob });
	else 
	if(d < 10000)
		dg.below_10000.insert({ d, ob });
	else
		dg.too_far.insert({ d, ob });
}
bool fill_Distance_Group(Distance_Group& dg,Group& ob)
{
	if (is_created_Distance_Group(dg)) return false;

	for (auto& a : ob) 
		insert_in_Distance_Group(dg, &a);
	return true;
}
wostream& operator<<(wostream& os, const Distance_Group& p)
{
	if(p.below_100.size())	os << "{<100}\n";
	for (const auto& a : p.below_100)	os << a.first<<' ' << *a.second << endl;
	
	if (p.below_1000.size())	os << "{<1000}\n";
	for (const auto& a : p.below_1000)	os << a.first << ' ' << *a.second << endl;
	
	if (p.below_10000.size()) os << "{<10000}\n";
	for (const auto& a : p.below_10000)	os << a.first << ' ' << *a.second << endl;
	
	if(p.too_far.size())	os << "{too_far}\n";
	for (const auto& a : p.too_far)	os << a.first << ' ' << *a.second << endl;
	
	return os;
}
//------------------------------------------------------------------------------
void insert_in_Time_Group(Time_Group& dg, Object* ob)
{
	double d = ob->time;
	
	time_t object_time = d;
	tm object_t{};
	localtime_s(&object_t, &object_time);
	int year{ 1900 + object_t.tm_year };
	int month{ object_t.tm_mon };
	int day{ object_t.tm_mday };
	
//	int year{ 2023 };	int month{ 8 };	int day{ 1 };

	time_t now = time(0);
	tm t{};
	localtime_s(&t, &now);
	int year_now{ 1900 + t.tm_year };
	int month_now{ t.tm_mon };
	int day_now{ t.tm_mday };
	int day_week_now{ day_now- t.tm_wday };
	
	if (year_now == year && month_now == month && day_now== day)
		dg.today.insert({ d, ob });
	else
	if (year_now == year && month_now == month && day_now-1 == day)
		dg.yesterday.insert({ d, ob });
	else
	if (year_now == year && month_now == month&& day_week_now<= day)
		dg.this_week.insert({ d, ob });
	else 
	if (year_now == year && month_now==month)
		dg.this_month.insert({ d, ob });
	else 
	if (year_now== year)
		dg.this_year.insert({ d, ob });
	else 
		dg.earlier.insert({ d, ob });
}
bool fill_Time_Group(Time_Group& dg, Group& ob)
{
	if (is_created_Time_Group(dg)) return false;
	
	for (auto& a : ob)
		insert_in_Time_Group(dg, &a);
	return true;
}
wostream& operator<<(wostream& os, const Time_Group& p)
{
	if(p.today.size())	os << "{today}\n";
	for (const auto& a : p.today) os << *a.second << endl;
	
	if (p.yesterday.size())	os << "{yesterday}\n";
	for (const auto& a : p.yesterday)	os << *a.second << endl;
	
	if (p.this_week.size())	os << "{this_week}\n";
	for (const auto& a : p.this_week)	os << *a.second << endl;
	
	if (p.this_month.size())	os << "{this_month}\n";
	for (const auto& a : p.this_month)	os << *a.second << endl;
	
	if (p.this_year.size())	os << "{this_year}\n";
	for (const auto& a : p.this_year)	os << *a.second << endl;
	
	if (p.earlier.size())	os << "{earlier}\n";
	for (const auto& a : p.earlier)	os << *a.second << endl;
	
	return os;
}
//------------------------------------------------------------------------------
void get_name_and_open_ifile(wifstream& io)
{
	for (wstring name; !io.is_open();) {
		wcout << "Enter file name: ";
		wcin >> name;
		io.open(name);
		if (!io) {
			wcout << "Error oppening the file. Try again.\n";
			io.clear();
		}
		else wcout << "File successfully opened.\n";
	}
}
wostream& cout_or_ofile()
{
	wcout << L"Enter file name or press enter for console output: ";
	wchar_t c{ };
	wcin.get(); // pass 1 enter pressed previously 
	while (!isgraph(c) && c != L'\n') c = wcin.get();
	if (c == L'\n') return wcout;

	wcin.putback(c);
	wstring name;
	wcin >> name;
	wcout << "File successfully opened.\n";
	return *(new wofstream{ name });
}
//------------------------------------------------------------------------------
bool is_created_Primary_Group(const Group& prymary_group)
{
	return prymary_group.size();
}
bool is_created_Distance_Group(const Distance_Group& group)
{
	return group.below_100.size() || group.below_1000.size()
		|| group.below_10000.size() || group.too_far.size() ;
}
bool is_created_Time_Group(const Time_Group& group)
{
	return group.today.size() || group.yesterday.size()
		|| group.this_week.size() || group.this_month.size()
		|| group.this_year.size() || group.earlier.size();
}
bool is_created_Name_Group(const Name_Group& group)
{
	return group.size();
}
bool is_created_Type_Group(const Type_Group& group)
{
	return group.size();
}
//------------------------------------------------------------------------------
int main()
try
{
	//setlocale(LC_ALL, "ru-Ru");	SetConsoleCP(1251); SetConsoleOutputCP(1251);

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	wifstream ifs;
	get_name_and_open_ifile(ifs);

	Group primary_group;
	Distance_Group distance_g;
	Time_Group time_g;
	Name_Group name_g;
	Type_Group type_g;

	while(wcin)
	{
		wcout <<L"-------------------------\n"
			<< L"Start function:\n"
			<< L"1.Create Primapy Group.\n"
			<< L"2.Print Primapy Group.\n"
			<< L"3.Create Distance Group.\n"
			<< L"4.Print Distance Group.\n"
			<< L"5.Create Time Group.\n"
			<< L"6.Print Time Group.\n"
			<< L"7.Create Name Group.\n"
			<< L"8.Print Name Group.\n"
			<< L"9.Create Type Group.\n"
			<< L"a.Print Type Group.\n"
			<< L"b.Create All Groups.\n"
			<< L"c.Print All Groups.\n"
			<< L"0.End.\n";
		wcout << L"> ";

		wchar_t ch;
		wcin >> ch;
		switch (ch) {
			case '1':
			{
				if (is_created_Primary_Group(primary_group)) {
					wcout << "Primapy Group already created.\n";
					break;
				}
				if(ifs >> primary_group)
					wcout << "Primapy Group successfully created.\n";
				else
					wcout << "Some Error.\n";
				break;
			}
			case '2':
			{
				if (cout_or_ofile() << primary_group)
					wcout << "Operation completed.\n";
				else wcout << "Some Error.\n";
				break;
			}
			case '3':
			{
				if (!is_created_Primary_Group(primary_group)) {
					wcout << "Error. Primary Group not created.\n";
					break;
				}
				if (fill_Distance_Group(distance_g, primary_group))
					wcout << "Distance Group successfully created.\n";
				else
					wcout << "Distance Group already created.\n";
				break;
			}
			case '4':
			{
				if (cout_or_ofile() << distance_g)
					wcout << "Operation completed.\n";
				else wcout << "Some Error.\n";
				break;
			}
			case '5':
			{
				if (!is_created_Primary_Group(primary_group)) {
					wcout << "Error. Primary Group not created.\n";
					break;
				}
				if(fill_Time_Group(time_g, primary_group))
					wcout << "Time Group successfully created.\n";
				else
					wcout << "Time Group already created.\n";
				break;
			}
			case '6':
			{
				if (cout_or_ofile() << time_g)
					wcout << "Operation completed.\n";
				else wcout << "Some Error.\n";
				break;
			}
			case '7':
			{
				if (!is_created_Primary_Group(primary_group)) {
					wcout << "Error. Primary Group not created.\n";
					break;
				}
				if(fill_Name_Group(name_g, primary_group))
					wcout << "Name Group successfully created.\n";
				else
					wcout << "Name Group already created.\n";
				break;
			}
			case '8':
			{
				if (cout_or_ofile() << name_g)
					wcout << "Operation completed.\n";
				else wcout << "Some Error.\n";
				break;
			}
			case '9':
			{
				if (!is_created_Primary_Group(primary_group)) {
					wcout << "Error. Primary Group not created.\n";
					break;
				}
				if(fill_Type_Group(type_g, primary_group))
					wcout << "Type Group successfully created.\n";
				else
					wcout << "Type Group already created.\n";
				break;
			}
			case 'a':
			{
				if (cout_or_ofile() << type_g)
					wcout << "Operation completed.\n";
				else wcout << "Some Error.\n";
				break;
			}
			case 'b':
			{
				//--
				if (is_created_Primary_Group(primary_group))
					wcout << "Primapy Group already created.\n";
				else
				if (ifs >> primary_group)
					wcout << "Primapy Group successfully created.\n";
				else
				{
					wcout << "Some Error.\n";
					break;
				}
				//--
				if (fill_Distance_Group(distance_g, primary_group))
					wcout << "Distance Group successfully created.\n";
				else
					wcout << "Distance Group already created.\n";
				//--
				if (fill_Time_Group(time_g, primary_group))
					wcout << "Time Group successfully created.\n";
				else
					wcout << "Time Group already created.\n";
				//--
				if (fill_Name_Group(name_g, primary_group))
					wcout << "Name Group successfully created.\n";
				else
					wcout << "Name Group already created.\n";
				//--
				if (fill_Type_Group(type_g, primary_group))
					wcout << "Type Group successfully created.\n";
				else
					wcout << "Type Group already created.\n";
				//--
				break;
			}
			case 'c':
			{
				wostream& os = cout_or_ofile();
				if( is_created_Primary_Group(primary_group))
					os << "[primary_group]\n" << primary_group;
				if (is_created_Distance_Group(distance_g))
					os << "[distance_group]\n" << distance_g;
				if (is_created_Time_Group(time_g))
					os << "[time_group]\n" << time_g;
				if (is_created_Name_Group(name_g))
					os << "[name_group]\n" << name_g;
				if (is_created_Type_Group(type_g))
					os << "[type_group]\n" << type_g;
				if(os)
					wcout << "Operation completed.\n";
				else 
					wcout << "Some Error.\n";
				break;
			}
			case '0': return 0;
			default:
			{
				wcout <<"You entered "<<  ch << ". Try another simbol.\n";
				break;
			}
		}
	}

	wcin.get();
	return 0;
}
catch (const std::exception& e)
{
	wcerr << "Error: ";
	wcerr << e.what() << ".\n";
	wcin.get();
}
catch (...)
{
	wcerr << "Some error.\n";
	wcin.get();
}

