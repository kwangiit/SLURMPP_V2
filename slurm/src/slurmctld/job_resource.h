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
	int num_ctrl;
	char **ctrl_ids;
	char **node_alloc;
	int *num_node_indi;
} job_resource;

typedef struct _free_resource {
	int num_free_node;
	char **free_nodelist;
} free_resource;

/* adding binary search tree*/
typedef struct _BST {
	char *data;
	int num;

	struct _BST *leftchild;
	struct _BST *rightchild;
} BST;

typedef struct _hashtable {
	uint32_t key;
	job_resource *value;
	struct _hashtable *next;
} hashtable;

typedef struct _resource_BST {
	int num_part;
	int num_free_nodes;
	BST *resource_bst;
} resource_BST;

extern resource_BST *global_res_BST;
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
extern void incre_free_resource(char*, free_resource*);

extern void init_job_hashtable();
extern unsigned int hash(uint32_t);
extern job_resource* get_value(uint32_t);

extern void BST_insert(BST**, char*, int);
extern BST* BST_search_best(BST**, int);
extern BST* BST_search_exact(BST**, char*);
extern BST* BST_delete(BST**, char*, int);
extern void BST_delete_all(BST**);
extern void BST_print_all(BST**);

#endif /* JOB_RESOURCE_H_ */
