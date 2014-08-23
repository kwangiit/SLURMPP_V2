#include "str_trim.h"

#include  <sstream>

str_trim::str_trim() {
}
str_trim::~str_trim() {
}

string str_trim::trim(const string& str) {

	string value = str;

	stringstream trimmer;

	trimmer << value;
	trimmer >> value;

	return value;
}

