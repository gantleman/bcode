// test_bcode.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "bcode.h"

char* bstr = "d1:rd1:ti997e2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:r1:fli996e5:hello5:worldee";

int _tmain(int argc, _TCHAR* argv[])
{
	int cur = 0;
	b_element out;
	b_parse(bstr, strlen(bstr), cur, out);

	char* o;
	int olen;
	b_find(&out, "t", &o, olen);

	std::string so;
	b_package(&out, so);
	return 0;
}

