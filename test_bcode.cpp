// test_bcode.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "bcode.h"

char* bstr = "d1:rd1:ti997e2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:r1:fli996e5:hello5:worldee";

std::string
test_package()
{
	b_element in, *ia, *d, *l;
	b_insert(&in, "y", (unsigned char*)"q", 1);
	b_insertd(&in, "a", &ia);
	b_insert(ia, "r", (unsigned char*)"q", 1);
	b_insertd(ia, "d", &d);
	for (int i = 0; i < 5; i++)
	{
		std::stringstream key, value;
		key << "k" << i;
		value << "v" << i;
		b_insert(d, key.str().c_str(), (unsigned char*)value.str().c_str(), value.str().size());
	}

	b_insertl(ia, "l", &l);
	for (int i = 0; i < 5; i++)
	{
		std::stringstream value;
		value << "v" << i;
		b_insert(l, "", (unsigned char*)value.str().c_str(), value.str().size());
	}
	std::string so;
	b_package(&in, so);
	return so;
}

void test_read(std::string &in)
{
	int cur = 0;
	b_element out, *y, *a, *r, *d, *od, *l, *lo;
	b_parse(in.c_str(), in.size(), cur, out);
	b_find(&out, "y", &y);
	b_find(&out, "a", &a);
	b_find(a, "r", &r);
	b_find(a, "d", &d);
	b_find(d, "k0", &od);
	for (int i = 0; i < 4; i++)
	{
		b_next(d, &od);
	}

	b_find(a, "l", &l);
	b_get(l, 0, &lo);
	for (int i = 0; i < 4; i++)
	{
		b_next(l, &lo);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int cur = 0;
	b_element out;
	b_parse(bstr, strlen(bstr), cur, out);

	unsigned char* o;
	int olen;
	b_find(&out, "t", &o, olen);

	std::string so;
	b_package(&out, so);

	std::string r = test_package();
	test_read(r);
	return 0;
}

