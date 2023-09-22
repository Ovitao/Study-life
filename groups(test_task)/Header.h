#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
using namespace std;

struct Point
{
	double x;
	double y;
};

struct Object
{
	wstring name;
	Point point;
	wstring type;
	double time;
};

using Group = vector<Object>;

wistream& operator>>(wistream& is, Point& p);
wistream& operator>>(wistream& is, Object& o);
wistream& operator>>(wistream& is, Group& o);

wostream& operator<<(wostream& os, const Point& p);
wostream& operator<<(wostream& os, const Object& p);
wostream& operator<<(wostream& os, const Group& p);

struct Distance_Group
{
	multimap<double, Object*> below_100;
	multimap<double, Object*> below_1000;
	multimap<double, Object*> below_10000;
	multimap<double, Object*> too_far;
};

struct Time_Group
{
	multimap<double, Object*> today;
	multimap<double, Object*> yesterday;
	multimap<double, Object*> this_week;
	multimap<double, Object*> this_month;
	multimap<double, Object*> this_year;
	multimap<double, Object*> earlier;
};

using Name_Group = map<wchar_t, multimap<wstring,Object*>>;
using Type_Group = map<wstring, multimap<wstring, Object*>>;

void insert_in_Name_Group(Name_Group& dg, Object* ob);
bool fill_Name_Group(Name_Group& dg, Group& ob);

void insert_in_Type_Group(Type_Group& dg, Object* ob);
bool fill_Type_Group(Type_Group& dg, Group& ob);

void insert_in_Distance_Group(Distance_Group& dg, Object* ob);
bool fill_Distance_Group(Distance_Group& dg, Group& ob);

void insert_in_Time_Group(Time_Group& dg, Object* ob);
bool fill_Time_Group(Time_Group& dg, Group& ob);

wostream& operator<<(wostream& os, const Distance_Group& p);
wostream& operator<<(wostream& os, const Type_Group& p);
wostream& operator<<(wostream& os, const Time_Group& p);
wostream& operator<<(wostream& os, const Name_Group& p);

bool is_created_Primary_Group(const Group& prymary_group);
bool is_created_Distance_Group(const Distance_Group& group);
bool is_created_Time_Group(const Time_Group& group);
bool is_created_Name_Group(const Name_Group& group);
bool is_created_Type_Group(const Type_Group& group);

