#pragma once
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <list>

class b_element
{
public:
	int type;
	std::vector<char> buf;

	b_element() :type(0){};
	~b_element();
};

void b_parse(const char* buf, int len, int &cur, b_element& out);
void b_find(b_element* e, const char* key, char** o, int& len);
void b_find(b_element* e, const char* key, b_element** o);
void b_add(b_element* e, const char* key, b_element** o);
int b_add(b_element* e, const char* key, char* i, int len);
void b_del(b_element* e, const char* key);
void b_package(b_element* e, std::string& o);
