/*
 * slurmutil.h
 *
 *  Created on: Jun 11, 2014
 *      Author: kwang
 */

#ifndef SLURMUTIL_H_
#define SLURMUTIL_H_

#include <stdlib.h>
#include <string.h>
#include <src/common/xmalloc.h>
#include "src/ZHT/src/c_zhtclient.h"
//#include "src/ZHT/src/meta.pb-c.h"

extern void c_memset(char*, int);
extern void xfree_2(char**, int);
extern char** xmalloc_2(int, int);
extern char* int_to_str(int);
extern int str_to_int(char*);
extern int split_str(char*, char*, char**);
extern int find_exist(char**, char*, int);

#endif /* SLURMUTIL_H_ */
