/*
 * job_resource.c
 *
 *  Created on: Jun 16, 2014
 *      Author: kwang
 */

#include <src/slurmctld/job_resource.h>

hashtable *job_hashtable[MAXHASHSIZE];

extern job_resource* init_job_resource()
{
	job_resource *a_job_res = xmalloc(sizeof(job_resource));
	//srand(time(NULL));
	pthread_mutex_lock(&num_job_recv_mutex);
	num_job_recv++;
	pthread_mutex_unlock(&num_job_recv_mutex);
	
	char *job_id_tmp = xmalloc(100);
	char *str_num_job_recv = xmalloc(20);
        sprintf(str_num_job_recv, "%d", num_job_recv);
	//char *self_index_str = xmalloc(20);
	//sprintf(self_index_str, "%d", self_index);

	strcat(job_id_tmp, str_num_job_recv);
	strcat(job_id_tmp, str_self_index);
	char **end = NULL;
	a_job_res->job_id = (unsigned int) (strtol(job_id_tmp, end, 10));
	
	xfree(job_id_tmp);
	xfree(str_num_job_recv);
	//xfree(self_index_str);
	//a_job_res->job_id = MIN_NOALLOC_JOBID +
	//		((uint32_t) rand() %
	//		 (MAX_NOALLOC_JOBID - MIN_NOALLOC_JOBID + 1));
	//printf("The job id now is:%u\n", a_job_res->job_id);
	a_job_res->num_try = 0;
	a_job_res->num_node = 0;
	a_job_res->sleep_length = 10000;
	a_job_res->nodelist = xmalloc(30 * part_size * num_ctrl);
	return a_job_res;
}

extern void reset_job_resource(job_resource *a_job_res)
{
	if (a_job_res != NULL) {
		a_job_res->num_try = 0;
		a_job_res->num_node = 0;
		a_job_res->sleep_length = 10000;
		c_memset(a_job_res->nodelist, 30 * part_size * num_ctrl);
	} else {
		a_job_res = xmalloc(sizeof(job_resource));
		a_job_res->num_try = 0;
		a_job_res->num_node = 0;
		a_job_res->sleep_length = 10000;
		a_job_res->nodelist = xmalloc(30 * part_size * num_ctrl);
	}
}

//int _get_index(char *ctrl_id)
//{
//	int i = 0;
//	for (; i < num_ctrl; i++) {
//		if (!strcmp(ctrl_id, mem_list[i])) {
//			break;
//		}
//	}
//	return i;
//}

void update_job_resource(job_resource *a_job_res,
						    int num_node_allocated,
						    char *nodelist)
{
	a_job_res->num_try = 0;
	a_job_res->sleep_length = 10000;
	if (a_job_res->num_node > 0) {
		strcat(a_job_res->nodelist, ",");
	}
	strcat(a_job_res->nodelist, nodelist);
	a_job_res->num_node += num_node_allocated;
}

int do_allocate(char *cur_free_res_str, job_resource *a_job_res,
		char *query_value, int num_more_node)
{
	free_resource *cur_free_res = unpack_free_resource(cur_free_res_str);
	int ret = 0;
	if (cur_free_res->num_free_node > 0) {
		int num_node_attempt = cur_free_res->num_free_node > num_more_node ?
				num_more_node : cur_free_res->num_free_node;
		int num_node_left = cur_free_res->num_free_node - num_node_attempt;
		char *nodelist = xmalloc(part_size * num_ctrl * 30);
		int i;
		for (i = 0; i < num_node_attempt; i++) {
			strcat(nodelist, cur_free_res->free_nodelist[i]);
			if (i != num_node_attempt - 1) {
				strcat(nodelist, ",");
			}
		}
		free_resource *new_free_res = xmalloc(sizeof(free_resource));
		new_free_res->num_free_node = num_node_left;
		if (num_node_left > 0) {
			new_free_res->free_nodelist = xmalloc_2(num_node_left, 30);
		}
		for (i = 0; i < num_node_left; i++) {
			strcat(new_free_res->free_nodelist[i],
					cur_free_res->free_nodelist[i + num_node_attempt]);
		}
		char *new_free_res_str = pack_free_resource(new_free_res);
		int ret = c_zht_compare_swap("number of free node",
				cur_free_res_str, new_free_res_str, query_value);
		if (ret == 0) {
			update_job_resource(a_job_res, num_node_attempt, nodelist);
		}else {
			ret = 1;
		}
		incre_zht_msg("cswap", 1);
		xfree(nodelist);
		dealloc_free_resource(new_free_res);
		xfree(new_free_res_str);
	} else {
		ret = -1;
	}
	dealloc_free_resource(cur_free_res);
	return ret;
}

void allocate_resource(job_resource *a_job_res, int num_more_node)
{
	char *cur_free_res_str = xmalloc((part_size * num_ctrl + 1) * 30);
	printf("Before doing the look up to allocate resource!\n");
	c_zht_lookup("number of free node", cur_free_res_str);
	printf("After doing the lookup, %s\n", cur_free_res_str);
	incre_zht_msg("lookup", 1);
	if (cur_free_res_str == NULL) {
		a_job_res->num_try++;
	}else {
		char *query_value = xmalloc((part_size * num_ctrl + 1) * 30);
		int ret = do_allocate(cur_free_res_str, a_job_res, query_value, num_more_node);
		while (ret == 1) {
			c_memset(cur_free_res_str, (part_size * num_ctrl + 1) * 30);
			strcpy(cur_free_res_str, query_value);
			c_memset(query_value, (part_size * num_ctrl + 1) * 30);
			ret = do_allocate(cur_free_res_str, a_job_res, query_value, num_more_node);
		}
		if (ret < 0) {
			a_job_res->num_try++;
			usleep(a_job_res->sleep_length);
			a_job_res->sleep_length *= 2;
		}
		xfree(cur_free_res_str);
		xfree(query_value);
	}
}

extern job_resource* allocate_job_resource(int num_node_req)
{
	job_resource *a_job_res = init_job_resource();

	while (a_job_res->num_node < num_node_req) {
		printf("OK, keep allocating!\n");
		allocate_resource(a_job_res, num_node_req - a_job_res->num_node);
		if (a_job_res->num_try > 10) {
			printf("I have tried more than 10 times!\n");
			release_job_resource(a_job_res);
			reset_job_resource(a_job_res);
			usleep(a_job_res->sleep_length);
		}
	}
	return a_job_res;
}

extern int insert_job_record(job_resource *a_job_res)
{
	if (a_job_res == NULL)
		return -1;
	char *str_job_id = xmalloc(20);
	sprintf(str_job_id, "%u", a_job_res->job_id);
	char *job_res = xmalloc((a_job_res->num_node + 1) * 30);
	char *num_node_str = int_to_str(a_job_res->num_node);
	strcat(job_res, num_node_str);
	strcat(job_res, ";");
	strcat(job_res, a_job_res->nodelist);

	int ret = put_recrod(str_job_id, job_res);
	xfree(str_job_id);
	xfree(job_res);
	xfree(num_node_str);

	return ret;
}

extern void release_job_resource(job_resource *a_job_res)
{
	if (a_job_res->num_node > 0) {
		free_resource *to_be_incre = xmalloc(sizeof(free_resource));
		to_be_incre->num_free_node = a_job_res->num_node;
		to_be_incre->free_nodelist = xmalloc_2(to_be_incre->num_free_node, 30);
		char *nodelist = strdup(a_job_res->nodelist);
		split_str(nodelist, ",", to_be_incre->free_nodelist);
		incre_free_resource(to_be_incre);
		free(nodelist);
		dealloc_free_resource(to_be_incre);
	}
	//dealloc_job_resource(a_job_res);
}

extern void dealloc_job_resource(job_resource *a_job_res)
{
	if (a_job_res != NULL) {
		xfree(a_job_res->nodelist);
		a_job_res->nodelist = NULL;
		xfree(a_job_res);
		a_job_res = NULL;
    }
}

extern void incre_zht_msg(char *msg_type, int incre)
{
	if (!strcmp(msg_type, "lookup")) {
		pthread_mutex_lock(&lookup_msg_mutex);
		num_lookup_msg += incre;
		pthread_mutex_unlock(&lookup_msg_mutex);
	}else if (!strcmp(msg_type, "insert")) {
		pthread_mutex_lock(&insert_msg_mutex);
		num_insert_msg += incre;
		pthread_mutex_unlock(&insert_msg_mutex);
	}else if (!strcmp(msg_type, "cswap")) {
		pthread_mutex_lock(&cswap_msg_mutex);
		num_cswap_msg += incre;
		pthread_mutex_unlock(&cswap_msg_mutex);
	}else if (!strcmp(msg_type, "callback")) {
		pthread_mutex_lock(&callback_msg_mutex);
		num_callback_msg += incre;
		pthread_mutex_unlock(&callback_msg_mutex);
	}
}

extern char* pack_free_resource(free_resource *a_free_resource)
{
	char *str_free_resource = xmalloc((a_free_resource->num_free_node + 1) * 30);
	char *str_num_free_node = int_to_str(a_free_resource->num_free_node);
	strcat(str_free_resource, str_num_free_node);
	strcat(str_free_resource, ";");
	int i;
	for (i = 0; i < a_free_resource->num_free_node; i++) {
		strcat(str_free_resource, a_free_resource->free_nodelist[i]);
		if (i != a_free_resource->num_free_node - 1) {
			strcat(str_free_resource, ",");
		}
	}
	xfree(str_num_free_node);
	return str_free_resource;
}

extern free_resource* unpack_free_resource(char *str)
{
	char *res[2];
	char *str_copy = strdup(str);
	split_str(str_copy, ";", res);
	free_resource *a_free_resource = xmalloc(sizeof(free_resource));
	int num_free_node = str_to_int(res[0]);
	a_free_resource->num_free_node = num_free_node;
	if (num_free_node > 0) {
		a_free_resource->free_nodelist = xmalloc_2(num_free_node, 30);
		char *p[num_free_node];
		split_str(res[1], ",", p);
		int i;
		for (i = 0; i < num_free_node; i++) {
			strcpy(a_free_resource->free_nodelist[i], p[i]);
		}
	}
	free(str_copy);
	return a_free_resource;
}

free_resource* merge_resource(
		free_resource *a_resource, free_resource *b_resource)
{
	free_resource *sum_resource = xmalloc(sizeof(free_resource));
	sum_resource->num_free_node = a_resource->num_free_node + b_resource->num_free_node;

	if (sum_resource->num_free_node > 0) {
		sum_resource->free_nodelist = xmalloc_2(sum_resource->num_free_node, 30);
		int i;
		for (i = 0; i < a_resource->num_free_node; i++) {
			strcpy(sum_resource->free_nodelist[i], a_resource->free_nodelist[i]);
		}
		for (i = 0; i < b_resource->num_free_node; i++) {
			strcpy(sum_resource->free_nodelist[i + a_resource->num_free_node],
					b_resource->free_nodelist[i]);
		}
	}
	return sum_resource;
}

extern void dealloc_free_resource(free_resource *a_resource)
{
	if (a_resource != NULL) {
		if (a_resource->num_free_node > 0) {
			xfree_2(a_resource->free_nodelist, a_resource->num_free_node);
		}
		xfree(a_resource);
		a_resource = NULL;
	}
}

extern void incre_free_resource(free_resource *to_be_incre)
{
	char *cur_free_res_str = xmalloc((num_ctrl * part_size + 1) * 30);
	while (1) {
		c_zht_lookup("number of free node", cur_free_res_str);
		if (cur_free_res_str != NULL) {
			incre_zht_msg("lookup", 1);
			break;
		}
		usleep(100);
	}
	free_resource *cur_free_res = unpack_free_resource(cur_free_res_str);
	free_resource *new_free_res = merge_resource(cur_free_res, to_be_incre);
	char *new_free_res_str = pack_free_resource(new_free_res);
	char *query = xmalloc((num_ctrl * part_size + 1) * 30);

	int num_cswap_msg_local = 1;

	while (c_zht_compare_swap("number of free node",
			cur_free_res_str, new_free_res_str, query) != 0) {
		num_cswap_msg_local++;
		c_memset(cur_free_res_str, (num_ctrl * part_size + 1) * 30);
		strcpy(cur_free_res_str, query);
		dealloc_free_resource(cur_free_res);
		dealloc_free_resource(new_free_res);
		xfree(new_free_res_str);
		cur_free_res = unpack_free_resource(cur_free_res_str);
		new_free_res = merge_resource(cur_free_res, to_be_incre);
		new_free_res_str = pack_free_resource(new_free_res);
		c_memset(query, (num_ctrl * part_size + 1) * 30);
	}
	xfree(cur_free_res_str);
	xfree(new_free_res_str);
	xfree(query);
	dealloc_free_resource(cur_free_res);
	dealloc_free_resource(new_free_res);
	incre_zht_msg("cswap", num_cswap_msg_local);
}

extern void init_job_hashtable()
{
	int i;
	for (i = 0; i < MAXHASHSIZE; i++) {
		job_hashtable[i] = NULL;
	}
}

extern unsigned int hash(char *key)
{
	unsigned int h = 0;
	for(; *key; key++)
		h = *key + h * 31;
	return h % MAXHASHSIZE;
}

hashtable* lookup(char *key)
{
	unsigned int hi = hash(key);
	hashtable *list = job_hashtable[hi];

	for (; list != NULL; list = list->next) {
		if (!strcmp(list->key, key)) {
				return list;
		}
	}

	return NULL;
}

extern char* get_value(char *key)
{
	hashtable *list = lookup(key);
	if (list == NULL)
		return NULL;
	else
		return strdup(list->value);
}

extern int put_recrod(char *key, char *value)
{
	unsigned int hi;
	hashtable *list;

	if ((list = lookup(key)) == NULL) {
		hi = hash(key);
		list = xmalloc(sizeof(hashtable));
		if (list == NULL)
			return 0;
		list->key = strdup(key);
		if (list->key == NULL)
			return 0;
		list->next = job_hashtable[hi];
		job_hashtable[hi] = list;
	} else
		free(list->value);
	list->value = strdup(value);
	if (list->value == NULL)
		return 0;

	return 1;
}
