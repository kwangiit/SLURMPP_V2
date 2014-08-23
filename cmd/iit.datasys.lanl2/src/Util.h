/*
 * Util.h
 *
 *  Created on: Feb 5, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <sys/types.h>
#include <string>
#include <sstream>

using namespace std;

namespace iit {
namespace datasys {
namespace lanl {

/*
 *
 */
class Util {
public:
	Util();
	virtual ~Util();

public:
	static double getTime_usec();

	static double getTime_msec();

	static uint getRandom(uint max);

public:
	static const uint FILE_MAX;

};

} /* namespace lanl */
} /* namespace datasys */
} /* namespace iit */

template<class TYPE> string toString(const TYPE& ele) {

	stringstream ss;
	ss << ele;

	return ss.str();
}

#endif /* UTIL_H_ */
