/*
 * job_resource.h
 *
 *  Created on: Jun 16, 2014
 *      Author: kwang
 */

#ifndef JOB_RESOURCE_H_
#define JOB_RESOURCE_H_

#include "src/slurmctld/slurmctld.h"
#define MAXHASHSIZE 10000

typedef struct _job_resource {
	uint32_t job_id;
	int num_try;
	int num_node;
	int sleep_length;
	char *nodelist;
} job_resource;

typedef struct _free_resource {
	int num_free_node;
	char **free_nodelist;
} free_resource;

typedef struct _hashtable {
	char *key;
	char *value;
	struct _hashtable *next;
} hashtable;

extern hashtable *job_hashtable[MAXHASHSIZE];

extern job_resource* init_job_resource();
extern void reset_job_resource(job_resource*);
extern job_resource* allocate_job_resource(int);
extern void release_job_resource(job_resource*);
extern void dealloc_job_resource(job_resource*);

extern int insert_job_record(job_resource*);
extern void incre_zht_msg(char*, int);

extern char* pack_free_resource(free_resource*);
extern free_resource* unpack_free_resource(char*);
extern void dealloc_free_resource(free_resource*);
extern void incre_free_resource(free_resource*);

extern void init_job_hashtable();
extern unsigned int hash(char*);
extern char* get_value(char*);
extern int put_pair(char*, char*);

#endif /* JOB_RESOURCE_H_ */
