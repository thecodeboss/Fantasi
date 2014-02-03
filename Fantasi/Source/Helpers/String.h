#ifndef String_h__
#define String_h__
#include <string>
#include <algorithm>
using namespace std;

static inline string &ltrim(string &s)
{
	s.erase(s.begin(),find_if_not(s.begin(),s.end(),[](int c){return isspace(c);}));
	return s;
}

static inline string &rtrim(string &s)
{
	s.erase(find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base(), s.end());
	return s;
}

static inline string trim(const string &s)
{
	string t=s;
	return ltrim(rtrim(t));
}

#endif // String_h__
