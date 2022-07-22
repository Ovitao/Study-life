#pragma once
// 7.7.22


/*
	uses single linked list without a dummy header
	skip list of ordered elements, no repeated elements on a level
*/
#include "../../../../Custom Header Files/std_lib_facilities.h"


class Link
{
public:
	string value;
	Link(const string& v, Link* s = nullptr)
		:value{ v }, succ{ s }{}

	Link* next() { return succ; }
	Link* set_next(Link* s) { succ = s; return succ; }
private:
	Link* succ;
};




class List
{
public:
	explicit List(Link* header = nullptr) : header{ header } { 
		//cout << "created constr " << this << '\n'; 
	}
	Link* add_ordered(Link*); // insert  Link into lexografical position. return given Link

	Link* erase(Link* erase_this); // remove this Link from Links if this link in the list returns succcesor of erased, if not returns 0.
	Link* erase(int erase_this); // remove n Link from Links.return return succcesor of erased

	Link* find(Link*); // find if the Link is in the list
	Link* find(const string&); // find Link with value in the list
	Link* link(int);

	List(const List&);
	const List& operator=(const List&);
	List(List&&);
	const List& operator=(List&&);
	virtual ~List();
private:
	Link* header = nullptr;
};


class Linked_List :public List
{
public:
	int val{ 0 };
	Linked_List(int val = 0, Link* header = nullptr, Linked_List* next = nullptr)
		:List{ header }, val{ val }, succ{ next } {}

	Linked_List* next() { return succ; }
	Linked_List* set_next(Linked_List* s) { succ = s; return succ; }
private:
	Linked_List* succ;
};

class Skip_List
{
public:
	explicit Skip_List(Linked_List* base = nullptr) :base{ base }, _lvls{ base ? 1 : 0 }{}

	Link* add(Link*); // ordered

	Link* find(const string&);

	Linked_List* lvl(int);
	int lvls() { return _lvls; }

	Skip_List(const Skip_List&);
	const Skip_List& operator=(const Skip_List&);
	Skip_List(List&&);
	const Skip_List& operator=(Skip_List&&);
	~Skip_List();
private:
	int _lvls;
	Linked_List* base;
};