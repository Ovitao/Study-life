
#include "Skip_list.h"

//-------------------------------------------------------------


Linked_List* Skip_List::lvl(int n)
{
	if (n < 0|| _lvls<n) error("out of range", n);

	Linked_List* ll= base;
	while (n--) ll = ll->next();
	return ll;
	//return next_times(base, n);
}

Link* Skip_List::find(const string& s)
{
	Link* l = nullptr;
	for (int n{ _lvls }; !l && --n; l = lvl(n)->find(s));
	return l;
}

Link* Skip_List::add(Link* l)
{
	if (!base) {  base = new Linked_List{ _lvls,l }; _lvls++; return l; }
	
	Linked_List* ll = base;
	Link* found = nullptr;
	while ((found=ll->find(l->value)) && ll->next()) ll = ll->next();

	if (!found)
		ll->add_ordered(l);
	else {
		ll->set_next(new Linked_List{ _lvls,l });
		_lvls++;
	}
	return l;
}
void delete_all_next(Linked_List* l)
{
	if (l->next())
		delete_all_next(l->next());

	delete l;
}
Skip_List::~Skip_List()
{
	if (base) delete_all_next(base);
}
/*
Linked_List* next_times(Linked_List*& base, int& l)
{
	if (l--) next_times(base, l)->next();
	return base;
}
Link* find_insertion_position(Linked_List*& ll, Link* l)
{
	int n{ 0 };
	Link* lk{ ll->link(0) };
	Link* lkp{ lk };
	while( lk->value > l->value && (lkp= lk )&& (lk = lk->next() ) );
	return lk;
}*/

//-------------------------------------------------------------

void delete_all_next(Link* l)
{
	if (l->next()) 
		  delete_all_next(l->next());

	delete l;
}
List::List(const List& lis)
{
//	cout <<"copy constr of " << this << "\n";
	operator=(lis);
}

List::~List()
{
	//cout << "destruction of " << this << "..."; 
	if (header) delete_all_next(header);
	
	//cout << "destructed .\n";
}
List::List(List&& l)
{
	//cout << &l << " moved to " << this << " List(List&& l)\n";
	header = l.header;
	l.header = nullptr;
}
const List& List::operator=(List&& l)
{
	Link* buffer = header;

	header = l.header;
	l.header = nullptr;
//	cout << &l << " moved to " << this << " operator=\n";
	if (buffer) delete_all_next(buffer);
	return *this;
}
const List& List::operator=(const List& l)
{
	Link* buffer = header;
//	cout << "copy assign of " << this << "\n";

	header = new Link{ *l.header };
	
	Link* from = l.header;
	if(from)
	for (Link* ths = header; from = from->next(); )
	{
		ths->set_next(new Link{ *from });
		ths = ths->next();
	}
	if (buffer) delete_all_next(buffer);
	return *this;
}


Link* List::erase(int erase_this) // remove this Link from Links. returns Link after erased
{
	return erase(link(erase_this));
}
Link* List::link(int lk) // remove this Link from Links. returns Link after erased
{
	if (lk < 0) error("link: out of range", lk);
	
	Link* p = header;
	for (int in = lk; p && in--; )p = p->next();
	if (!p) error("link: out of range", lk);
	return p;
}/*

Link* List::link(int lk) 
{
	if (lk < 0) error("link: out of range", lk);
	Link* p = header;
	int in = lk;
	while (p || in--)p = p->next();
	if (!p) error("link: out of range", lk);
	return p;
}

Link* List::link(int lk)
{
	if (lk--) return link(lk)->next();
	return header;
}
*/
Link* find_previous(Link* first, Link* lnk)
{
	if(first == lnk) return first;
	while (first->next() && first->next() != lnk) first = first->next();
	if (!first->next()) return nullptr;
	return first;
}

Link* List::erase(Link* erase_this) // remove this Link from Links. returns Link after erased
{
	Link* l = find_previous(header, erase_this);
	if (l)
	{
		if (l == header) header = header->next();
		else l->set_next(erase_this->next());
		return erase_this->next();
	}
	return nullptr;
}
Link* List::find(Link* l)  // find Link in the list
{
	Link* p = header;
	while (p != l && p != nullptr)
		p = p->next();
	return p;
}
Link* List::find(const string& val)  // find Link with value in the list
{
	Link* l = header;
	while (l && l->value != val)  l = l->next();
	return l;
}

Link* List::add_ordered(Link* l)  // insert  Link into lexografical position. return given Link
{
	if (!l) return l;
	if (!header || header->value > l->value) { l->set_next(header); header = l; return l; }
	Link* p = header;
	while (p->next() && l->value > p->next()->value) p = p->next();
	l->set_next(p->next());
	p->set_next(l);
	return l;
}
//-----------------------------------------------------------------






int main()
try
{


	{	
		Skip_List sl;
	
	
	/*Link ll{"vvv",new Link{"aaa"} };
	List lst{ new Link{"1",new Link{"2",new Link{"3"}}} };
	List lst3{ new Link{"s}"} };
	cout << ';';
	lst = lst3;
	cout << "Crea;\n";
	List lst2{ new Link{"a",new Link{"b"}} };
	cout << lst.link(1)->value << ' ';
	cout << lst2.link(1)->value << '\n';
	lst2 = lst;
	cout << lst.link(1)->value << ' ';
	cout << lst2.link(1)->value << '\n';
	lst.link(1)->value = "new val";
	cout << lst.link(1)->value << ' ';
	cout << lst2.link(1)->value << '\n';
	keep_window_open("~");*/
	for (int n{ 0 } ;n <50;++n)
			sl.add(new Link{ { char(randint('0', '9'))} });
		
	Linked_List* base = sl.lvl(0);
	cout << "Skip_List created;\n";
		cout << "LVLS "<< sl.lvls()<<'\n';
		for (size_t i = sl.lvls(); i-- ; )
		{
			Linked_List* ll = sl.lvl(i);
			cout << "Level " << ll->val <<". { ";
			Link* bl = base->link(0);
			for (Link* l = ll->link(0); l; )
			{
				while(bl &&l->value != bl->value)
				{
					bl = bl->next();
					cout << "   ";
					
				}
				bl = bl->next(); 
				cout << l->value << ", ";
				l = l->next();
			}
			cout << "}\n";
		}
	}

	keep_window_open("~");
}
catch (const std::exception& e)
{
	cerr << "Error:  " << e.what() << ".\n";
	keep_window_open("~");
}

