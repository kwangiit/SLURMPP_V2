/*
 * Util.cpp
 *
 *  Created on: Feb 5, 2013
 *      Author: Xiaobing Zhou
 */

#include "Util.h"

#include <sys/time.h>
#include <stdlib.h>

namespace iit {
namespace datasys {
namespace lanl {

const uint Util::FILE_MAX = 1000000000;

Util::Util() {
}

Util::~Util() {
}

double Util::getTime_usec() {

	struct timeval tp;

	gettimeofday(&tp, NULL);
	return static_cast<double>(tp.tv_sec) * 1E6
			+ static_cast<double>(tp.tv_usec);
}

double Util::getTime_msec() {

	struct timeval tp;

	gettimeofday(&tp, NULL);
	return static_cast<double>(tp.tv_sec) * 1E3
			+ static_cast<double>(tp.tv_usec) / 1E3;

}

uint Util::getRandom(uint max) {

	return rand() % max;
}

} /* namespace lanl */
} /* namespace datasys */
} /* namespace iit */
