#ifndef STR_TRIM_H
#define STR_TRIM_H

#include <string>

using namespace std;

class str_trim {

public:
	str_trim();
	virtual ~str_trim();

public:
	static string trim(const string& str);
};
#endif
