#include "bcode.h"

b_element::~b_element()
{
	if (type == 1)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &buf[0], sizeof(void*));
		delete p;
	}else if (type == 2)
	{
		std::list<b_element>* p;
		memcpy(&p, &buf[0], sizeof(void*));
		delete p;
	}
}

void b_parsel(const char* buf, int len, int &cur, b_element& out);

void b_parsem(const char* buf, int len, int &cur, b_element& out)
{
	out.type = 1;
	std::map<std::string, b_element>* p = new std::map<std::string, b_element>;
	out.buf.resize(sizeof(void*));
	memcpy(&out.buf[0], &p, sizeof(void*));

	if (buf[cur] != 'd')
		return;

	std::string key, slen;
	int kv = 0;//0 == key

	while (true)
	{
		if(cur++ == len)
			break;
		
		if (buf[cur] == 'd')
		{
			///跳入嵌套			
			b_parsem(buf, len, cur, (*p)[key]);
			kv = 0;
			key.clear();
		}else if (buf[cur] == 'e')
		{
			///跳出嵌套
			return;
		}else if (buf[cur] == 'l')
		{
			///跳入嵌套			
			b_parsel(buf, len, cur, (*p)[key]);
			kv = 0;
			key.clear();
		}
		else if (buf[cur] == ':')
		{
			int nlen;
			cur++;
			nlen = atoi(slen.c_str());
			if (0 == kv)
			{
				key.append((buf + cur), nlen);
				kv = 1;	
			}
			else
			{
				///复制数据到缓冲区
				(*p)[key].buf.resize(nlen);
				memcpy(&(*p)[key].buf[0], (buf + cur), nlen);
				kv = 0;	
				key.clear();
			}
			
			cur += nlen - 1;
			slen.clear();
		}else
			slen += buf[cur];
	}
}

void b_parsel(const char* buf, int len, int &cur, b_element& out)
{
	out.type = 2;
	std::list<b_element>* p = new std::list<b_element>;
	out.buf.resize(sizeof(void*));
	memcpy(&out.buf[0], &p, sizeof(void*));

	if (buf[cur] != 'l')
		return;

	std::string slen;
	while (true)
	{
		if (cur++ == len)
			break;

		if (buf[cur] == 'd')
		{
			///跳入嵌套			
			b_parsem(buf, len, cur, (*p).back());
		}
		else if (buf[cur] == 'e')
		{
			///跳出嵌套
			return;
		}else if (buf[cur] == 'l')
		{
			///跳入嵌套			
			b_parsel(buf, len, cur, (*p).back());
		}
		else if (buf[cur] == ':')
		{
			///复制数据到缓冲区
			int nlen;
			cur++;
			nlen = atoi(slen.c_str());

			///插入空值
			b_element t;
			(*p).push_back(t);
				
			(*p).back().buf.resize(nlen);
			memcpy(&(*p).back().buf[0], (buf + cur), nlen);

			cur += nlen - 1;
			slen.clear();
		}
		else
			slen += buf[cur];
	}
}

void b_parse(const char* buf, int len, int &cur, b_element& out)
{
	if (buf[cur] == 'l')
	{
		b_parsel(buf, len, cur, out);
	}
	else if (buf[cur] == 'd')
	{
		b_parsem(buf, len, cur, out);
	}
}

void b_find(b_element* e, const char* key, b_element** o)
{
	*o = 0;
	if (1 == e->type)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		std::map<std::string, b_element>::iterator iter = p->find(key);
		if (iter != p->end())
		{
			*o = &iter->second;
		}
	}

	return;
}


void b_find(b_element* e, const char* key, char** o, int& len)
{
	*o = 0;
	if (1 == e->type)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		std::map<std::string, b_element>::iterator iter = p->find(key);
		if (iter != p->end())
		{
			*o = &iter->second.buf[0];
			len = iter->second.buf.size();
		}
	}

	return;
}

void b_add(b_element* e, const char* key, b_element** o)
{
	*o = 0;
	if (1 == e->type)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		std::map<std::string, b_element>::iterator iter = p->find(key);
		if (iter == p->end())
		{
			(*p)[key].type = 1;
			*o = &(*p)[key];

			std::map<std::string, b_element>* np = new std::map<std::string, b_element>;
			(*o)->buf.resize(sizeof(void*));
			memcpy(&(*o)->buf[0], &np, sizeof(void*));
		}
	}

	return;
}

int b_add(b_element* e, const char* key, char* i, int len)
{
	if (1 == e->type)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		std::map<std::string, b_element>::iterator iter = p->find(key);
		if (iter == p->end())
		{
			(*p)[key].buf.resize(len);
			memcpy(&(*p)[key].buf[0], i, len);
			return 1;
		}
	}

	return 0;
}

void b_del(b_element* e, const char* key)
{
	if (1 == e->type)
	{
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		std::map<std::string, b_element>::iterator iter = p->find(key);
		if (iter != p->end())
		{
			p->erase(iter);
		}
	}
}

void b_packagel(b_element* e, std::string& o);

void b_packagem(b_element* e, std::string& o)
{
	if (1 == e->type)
	{
		o.append("d");
		std::map<std::string, b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		for (std::map<std::string, b_element>::iterator iter = p->begin(); iter != p->end(); iter++)
		{
			if (1 == iter->second.type)
			{
				std::stringstream ss;
				ss << iter->first.size();
				ss << ":";
				ss << iter->first;
				o.append(ss.str());
				b_packagem(&iter->second, o);
			}else if (2 == iter->second.type)
			{
				std::stringstream ss;
				ss << iter->first.size();
				ss << ":";
				ss << iter->first;
				o.append(ss.str());
				b_packagel(&iter->second, o);
			}
			else
			{
				std::stringstream ss;
				ss << iter->first.size();
				ss << ":";
				ss << iter->first;
				ss << iter->second.buf.size();
				ss << ":";
				o.append(ss.str());
				o.append(&iter->second.buf[0], iter->second.buf.size());
			}
		}
		o.append("e");
	}
}

void b_packagel(b_element* e, std::string& o)
{
	if (2 == e->type)
	{
		o.append("l");
		std::list<b_element>* p;
		memcpy(&p, &e->buf[0], sizeof(void*));

		for (std::list<b_element>::iterator iter = p->begin(); iter != p->end(); iter++)
		{
			if (1 == iter->type)
			{
				b_packagem(&(*iter), o);
			} else if (2 == iter->type)
			{
				b_packagel(&(*iter), o);
			}
			else
			{
				std::stringstream ss;
				ss << iter->buf.size();
				ss << ":";
				o.append(ss.str());
				o.append(&iter->buf[0], iter->buf.size());
			}
		}
		o.append("e");
	}
}

void b_package(b_element* e, std::string& o)
{
	if (1 == e->type)
	{
		b_packagem(e, o);
	}else if (2 == e->type)
	{
		b_packagel(e, o);
	}
}