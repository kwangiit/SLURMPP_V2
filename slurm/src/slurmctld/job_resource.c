/*
 * job_resource.c
 *
 *  Created on: Jun 16, 2014
 *      Author: kwang
 */

#include "src/slurmctld/job_resource.h"

hashtable *job_hashtable[MAXHASHSIZE];

extern job_resource* init_job_resource()
{
	job_resource *a_job_res = xmalloc(sizeof(job_resource));
	pthread_mutex_lock(&num_job_recv_mutex);
	num_job_recv++;
	pthread_mutex_unlock(&num_job_recv_mutex);
	
	char *job_id_tmp = xmalloc(100);
	char *str_num_job_recv = int_to_str(num_job_recv);
	strcat(job_id_tmp, str_num_job_recv);
	strcat(job_id_tmp, str_self_index);
	char **end = NULL;
	a_job_res->job_id = (unsigned int) (strtol(job_id_tmp, end, 10));
	xfree(job_id_tmp);
	xfree(str_num_job_recv);
	a_job_res->num_try = 0;
	a_job_res->num_node = 0;
	a_job_res->sleep_length = 10000;
	a_job_res->nodelist = xmalloc(30 * part_size * num_ctrl);
	a_job_res->num_ctrl = 0;
	a_job_res->ctrl_ids = xmalloc_2(num_ctrl, 30);
	a_job_res->node_alloc = xmalloc_2(num_ctrl, 30 * part_size);
	a_job_res->num_node_indi = xmalloc(sizeof(int) * num_ctrl);
	int i;
	for (i = 0; i < num_ctrl; i++) {
		a_job_res->num_node_indi[i] = 0;
	}

	return a_job_res;
}

extern void reset_job_resource(job_resource *a_job_res)
{
	if (a_job_res != NULL) {
		a_job_res->num_try = 0;
		a_job_res->num_node = 0;
		a_job_res->sleep_length = 10000;
		a_job_res->num_ctrl = 0;
		c_memset(a_job_res->nodelist, 30 * part_size * num_ctrl);
		int i;
		for (i = 0; i < num_ctrl; i++) {
			c_memset(a_job_res->ctrl_ids[i], 30);
			c_memset(a_job_res->node_alloc[i], 30 * part_size);
			a_job_res->num_node_indi[i] = 0;
		}
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
						    char *ctrl_id,
						    char *nodelist)
{
	a_job_res->num_try = 0;
	a_job_res->sleep_length = 10000;
	//a_job_res->num_node += num_node_allocated;
	if (a_job_res->num_node > 0) {
		strcat(a_job_res->nodelist, ",");
	}
	a_job_res->num_node += num_node_allocated;
	strcat(a_job_res->nodelist, nodelist);
	//printf("The nodelist allocated is:%s\n", nodelist);
	int index = find_exist(a_job_res->ctrl_ids, ctrl_id, a_job_res->num_ctrl);
	if (index < 0) {
		strcat(a_job_res->ctrl_ids[a_job_res->num_ctrl], ctrl_id);
		strcat(a_job_res->node_alloc[a_job_res->num_ctrl], nodelist);
		a_job_res->num_node_indi[a_job_res->num_ctrl] += num_node_allocated;
		//printf("The number of nodes is:%d\n", a_job_res->num_node_indi[a_job_res->num_ctrl]);
		a_job_res->num_ctrl++;
	} else {
		strcat(a_job_res->node_alloc[index], ",");
		strcat(a_job_res->node_alloc[index], nodelist);
		a_job_res->num_node_indi[index] += num_node_allocated;
	}
}

int do_allocate(char *free_res,
				  char *ctrl_id,
				  job_resource *a_job_res,
				  char *query_value,
				  int num_more_node)
{
	free_resource *cur_free_res = unpack_free_resource(free_res);
	int ret = 0;
	if (cur_free_res->num_free_node > 0) {
		int num_node_attempt = cur_free_res->num_free_node > num_more_node ?
				num_more_node : cur_free_res->num_free_node;
		int num_node_left = cur_free_res->num_free_node - num_node_attempt;
		char *nodelist = xmalloc(part_size * 30);
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
		char *str_new_free_res = pack_free_resource(new_free_res);
		int ret = c_zht_compare_swap(ctrl_id,
				free_res, str_new_free_res, query_value);
		if (ret == 0) {
			update_job_resource(a_job_res, num_node_attempt, ctrl_id, nodelist);
		}else {
			ret = 1;
		}
		incre_zht_msg("cswap", 1);
		xfree(nodelist);
		dealloc_free_resource(new_free_res);
		xfree(str_new_free_res);
	} else {
		ret = -1;
	}
	dealloc_free_resource(cur_free_res);
	return ret;
}

void allocate_resource(job_resource *a_job_res, char *ctrl_id, int num_more_node)
{
	char *free_res = xmalloc((part_size + 1) * 30);
	c_zht_lookup(ctrl_id, free_res);
	incre_zht_msg("lookup", 1);

	if (free_res == NULL) {
		a_job_res->num_try++;
	}else {
		char *query_value = xmalloc((part_size + 1) * 30);
		int ret = do_allocate(free_res, ctrl_id, a_job_res, query_value, num_more_node);
		while (ret == 1) {
			c_memset(free_res, (part_size + 1) * 30);
			strcpy(free_res, query_value);
			c_memset(query_value, (part_size + 1) * 30);
			ret = do_allocate(free_res, ctrl_id, a_job_res, query_value, num_more_node);
		}
		if (ret < 0) {
			a_job_res->num_try++;
			//usleep(a_job_res->sleep_length);
			//a_job_res->sleep_length *= 2;
		} else {
			//printf("I succeeded to allocate resource, num more node "
			//		"is:%d, all node is:%d\n", num_more_node, a_job_res->num_node);
			free_resource *cur_free_res = unpack_free_resource(query_value);
			pthread_mutex_lock(&global_res_BST_mutex);
			BST *bst = BST_search_exact(&(global_res_BST->resource_bst), ctrl_id);
			if (bst != NULL)
				BST_delete(&(global_res_BST->resource_bst), ctrl_id, bst->num);
			BST_insert(&(global_res_BST->resource_bst), ctrl_id, cur_free_res->num_free_node);
			pthread_mutex_unlock(&global_res_BST_mutex);
			dealloc_free_resource(cur_free_res);
		}
		xfree(free_res);
		xfree(query_value);
	}
}

extern job_resource* allocate_job_resource(int num_node_req)
{
	job_resource *a_job_res = init_job_resource();
	//char *ctrl_name = self_name;
	BST *bst = NULL;
	char *candidate_ctrl = xmalloc(30);
	//int num = -1;
	strcpy(candidate_ctrl, self_name);
	/*pthread_mutex_lock(&global_res_BST_mutex);
	bst = BST_search_exact(&(global_res_BST->resource_bst), ctrl_name);
	if (bst != NULL) {
		strcpy(candidate_ctrl, bst->data);
		num = bst->num;
		BST_delete(&(global_res_BST->resource_bst), bst->data, bst->num);
	}
	pthread_mutex_unlock(&global_res_BST_mutex);*/

	/*while (bst == NULL) {
		usleep(10000);
		pthread_mutex_lock(&global_res_BST_mutex);
		bst = BST_search_best(&(global_res_BST->resource_bst), num_node_req);
		if (bst != NULL) {
			c_memset(candidate_ctrl, 30);
			strcpy(candidate_ctrl, bst->data);
			num = bst->num;
			//printf("The data is:%s, and the number is:%d\n", bst->data, bst->num);
			BST_delete(&(global_res_BST->resource_bst), bst->data, bst->num);
		} else {
			//printf("OK, that is weired!\n");
			sleep(10);
		}
		pthread_mutex_unlock(&global_res_BST_mutex);
	}*/

	while (a_job_res->num_node < num_node_req) {
		//printf("OK, keep allocating!\n");
		//printf("num node req is:%d, allocated is:%d, candidate controller "
		//		"is:%s\n", num_node_req, a_job_res->num_node, candidate_ctrl);
		allocate_resource(a_job_res, candidate_ctrl, num_node_req - a_job_res->num_node);
		if (a_job_res->num_try > 10) {
			//printf("I have tried more than 10 times!\n");
			release_job_resource(a_job_res);
			reset_job_resource(a_job_res);
			usleep(a_job_res->sleep_length);
		}
		if (a_job_res->num_node < num_node_req) {
			c_memset(candidate_ctrl, 30);
			pthread_mutex_lock(&global_res_BST_mutex);
			bst = BST_search_best(&(global_res_BST->resource_bst),
					num_node_req - a_job_res->num_node);
			if (bst != NULL) {
				strcpy(candidate_ctrl, bst->data);
				//num = bst->num;
				BST_delete(&(global_res_BST->resource_bst), bst->data, bst->num);
			} else
				strcpy(candidate_ctrl, self_name);
			pthread_mutex_unlock(&global_res_BST_mutex);
			/*while (bst == NULL && a_job_res->num_node < num_node_req) {
				usleep(10000);
				pthread_mutex_lock(&global_res_BST_mutex);
				bst = BST_search_best(&(global_res_BST->resource_bst),
						num_node_req - a_job_res->num_node);
				if (bst != NULL) {
					c_memset(candidate_ctrl, 30);
					strcpy(candidate_ctrl, bst->data);
					//num = bst->num;
					BST_delete(&(global_res_BST->resource_bst), bst->data, bst->num);
				}
				pthread_mutex_unlock(&global_res_BST_mutex);
			}*/
		}
	}
	xfree(candidate_ctrl);
	return a_job_res;
}

job_resource *copy_job_resource(job_resource *a_job_resource)
{
	job_resource *b_job_resource = xmalloc(sizeof(job_resource));
	b_job_resource->job_id = a_job_resource->job_id;
	//printf("The job id is:%u\n", b_job_resource->job_id);
	b_job_resource->num_ctrl = a_job_resource->num_ctrl;
	//printf("The number of controller is:%d\n", b_job_resource->num_ctrl);
	b_job_resource->num_node = a_job_resource->num_node;
	//printf("The number of node is:%d\n", b_job_resource->num_node);
	b_job_resource->num_try = a_job_resource->num_try;
	//printf("The number of retry is:%d\n", b_job_resource->num_try);
	b_job_resource->sleep_length = a_job_resource->sleep_length;
	//printf("The sleep length is:%d\n", b_job_resource->sleep_length);

	b_job_resource->nodelist = xmalloc(30 * part_size * num_ctrl);
	strcpy(b_job_resource->nodelist, a_job_resource->nodelist);
	//printf("The node list now is:%s\n", b_job_resource->nodelist);
	b_job_resource->ctrl_ids = xmalloc_2(num_ctrl, 30);
	b_job_resource->node_alloc = xmalloc_2(num_ctrl, 30 * part_size);
	b_job_resource->num_node_indi = xmalloc(sizeof(int) * num_ctrl);

	int i;
	for (i = 0; i < b_job_resource->num_ctrl; i++) {
		strcpy(b_job_resource->ctrl_ids[i], a_job_resource->ctrl_ids[i]);
		//printf("The %dth controller is:%s\n", i, b_job_resource->ctrl_ids[i]);
		strcpy(b_job_resource->node_alloc[i], a_job_resource->node_alloc[i]);
		//printf("The %dth nodes are:%s\n", i, b_job_resource->node_alloc[i]);
		b_job_resource->num_node_indi[i] = a_job_resource->num_node_indi[i];
		//printf("The %dth number of nodes is:%d\n", i, b_job_resource->num_node_indi[i]);
	}

	return b_job_resource;
}

hashtable* lookup(uint32_t key)
{
	unsigned int hi = hash(key);
	hashtable *list = job_hashtable[hi];

	for (; list != NULL; list = list->next) {
		if (list->key == key) {
				return list;
		}
	}

	return NULL;
}

int put_record(uint32_t key, job_resource *value)
{
	unsigned int hi;
	hashtable *list;
	//printf("Now, insert job resource to local key-value store:%u, and nodelist is:%s\n", value->job_id, value->nodelist);
	if ((list = lookup(key)) == NULL) {
		//printf("OK, this is a new key %u and value\n", key);
		hi = hash(key);
		list = xmalloc(sizeof(hashtable));
		if (list == NULL)
			return 0;
		list->key = key;
		list->next = job_hashtable[hi];
		job_hashtable[hi] = list;
	} else
		dealloc_job_resource(list->value);
	list->value = copy_job_resource(value);
	if (list->value == NULL)
		return 0;

	return 1;
}

extern int insert_job_record(job_resource *a_job_res)
{
	if (a_job_res == NULL)
		return -1;
	int ret = put_record(a_job_res->job_id, a_job_res);
	return ret;
}

extern void release_job_resource(job_resource *a_job_res)
{
	if (a_job_res->num_ctrl > 0) {
		int i, j;
		for (i = 0; i < a_job_res->num_ctrl; i++) {
			free_resource *to_be_incre = xmalloc(sizeof(free_resource));
			to_be_incre->num_free_node = a_job_res->num_node_indi[i];
			to_be_incre->free_nodelist = xmalloc_2(to_be_incre->num_free_node, 30);
			char *p[to_be_incre->num_free_node];
			char *nodelist = strdup(a_job_res->node_alloc[i]);
			split_str(nodelist, ",", p);
			for (j = 0; j < to_be_incre->num_free_node; j++) {
				strcpy(to_be_incre->free_nodelist[j], p[j]);
			}
			free(nodelist);
			incre_free_resource(a_job_res->ctrl_ids[i], to_be_incre);
			dealloc_free_resource(to_be_incre);
		}
	}
	//dealloc_job_resource(a_job_res);
}

extern void dealloc_job_resource(job_resource *a_job_res)
{
	if (a_job_res != NULL) {
		xfree(a_job_res->nodelist);
		int i;
		for (i = 0; i < a_job_res->num_ctrl; i++) {
			xfree(a_job_res->ctrl_ids[i]);
			xfree(a_job_res->node_alloc[i]);
		}
		xfree(a_job_res->ctrl_ids);
		xfree(a_job_res->node_alloc);
		xfree(a_job_res->num_node_indi);
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

extern void incre_free_resource(char *ctrl_id, free_resource *to_be_incre)
{
	char *cur_free_res_str = xmalloc((part_size + 1) * 30);
	c_zht_lookup(ctrl_id, cur_free_res_str);
	incre_zht_msg("lookup", 1);

	free_resource *cur_free_res = unpack_free_resource(cur_free_res_str);
	free_resource *new_free_res = merge_resource(cur_free_res, to_be_incre);
	char *new_free_res_str = pack_free_resource(new_free_res);
	char *query = xmalloc((part_size + 1) * 30);

	int num_cswap_msg_local = 1;

	while (c_zht_compare_swap(ctrl_id, cur_free_res_str,
			new_free_res_str, query) != 0) {
		num_cswap_msg_local++;
		c_memset(cur_free_res_str, (part_size + 1) * 30);
		strcpy(cur_free_res_str, query);
		dealloc_free_resource(cur_free_res);
		dealloc_free_resource(new_free_res);
		xfree(new_free_res_str);
		cur_free_res = unpack_free_resource(cur_free_res_str);
		new_free_res = merge_resource(cur_free_res, to_be_incre);
		new_free_res_str = pack_free_resource(new_free_res);
		c_memset(query, (part_size + 1) * 30);
	}

	pthread_mutex_lock(&global_res_BST_mutex);
	BST *bst = BST_search_exact(&(global_res_BST->resource_bst), ctrl_id);
	if (bst != NULL)
		BST_delete(&(global_res_BST->resource_bst), bst->data, bst->num);
	BST_insert(&(global_res_BST->resource_bst),
			ctrl_id, new_free_res->num_free_node);
	pthread_mutex_unlock(&global_res_BST_mutex);
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

extern unsigned int hash(uint32_t key)
{
	unsigned int h = 0;
	char *str_key = xmalloc(30);
	sprintf(str_key, "%u", key);

	for(; *str_key; str_key++)
		h = *str_key + h * 31;

	//xfree(str_key);
	return h % MAXHASHSIZE;
}

extern job_resource* get_value(uint32_t key)
{
	hashtable *list = lookup(key);
	if (list == NULL)
		return NULL;
	else
	{
		return list->value;
	}
}

extern void BST_insert(BST **bst, char *data, int num)
{
	if (*bst == NULL) {
		*bst = xmalloc(sizeof(BST));
		(*bst)->data = xmalloc(30);
		strcpy((*bst)->data, data);
		//printf("The data is:%s\n", (*bst)->data);
		(*bst)->num = num;
		(*bst)->leftchild = (*bst)->rightchild = NULL;
	} else if (num < (*bst)->num) {
		BST_insert(&((*bst)->leftchild), data, num);
	} else {
		BST_insert(&((*bst)->rightchild), data, num);
	}
}

extern BST* BST_search_best(BST **bst, int num)
{
	if (*bst == NULL) {
		return NULL;
	}

	BST *candidate = *bst;
	BST *tmp = *bst;

	while (tmp != NULL) {
		if (tmp->num >= num) {
			candidate = tmp;
			tmp = tmp->leftchild;
		} else {
			tmp = tmp->rightchild;
			if (tmp != NULL && candidate->num < tmp->num)
				candidate = tmp;
		}
	}

	return candidate;
}

extern BST* BST_search_exact(BST **bst, char *data)
{
	if (*bst == NULL)
		return NULL;

	if (!strcmp((*bst)->data, data))
		return *bst;

	BST *candidate = BST_search_exact(&((*bst)->leftchild), data);
	if (candidate == NULL)
		return BST_search_exact(&((*bst)->rightchild), data);
	else
		return candidate;
}

BST *findMAX(BST **bst)
{
	if (*bst == NULL)
		return NULL;
	BST *tmp = *bst;
	while (tmp->rightchild != NULL)
		tmp = tmp->rightchild;
	return tmp;
}

extern BST* BST_delete(BST **bst, char *data, int num)
{
	BST *tmp;
	if (*bst == NULL)
		return NULL;
	if ((*bst)->num > num)
		(*bst)->leftchild = BST_delete(&((*bst)->leftchild), data, num);
	else if ((*bst)->num < num)
		(*bst)->rightchild = BST_delete(&((*bst)->rightchild), data, num);
	else if (strcmp((*bst)->data, data)) {
		(*bst)->rightchild = BST_delete(&((*bst)->rightchild), data, num);
	} else {
		if ((*bst)->rightchild && (*bst)->leftchild) {
			tmp = findMAX(&((*bst)->leftchild));
			memset((*bst)->data, '\0', 30);
			strcpy((*bst)->data, tmp->data);
			(*bst)->num = tmp->num;
			(*bst)->leftchild = BST_delete(
					&((*bst)->leftchild), tmp->data, tmp->num);
		} else {
			tmp = *bst;
			if ((*bst)->leftchild == NULL)
				*bst = (*bst)->rightchild;
			else if ((*bst)->rightchild == NULL)
				*bst = (*bst)->leftchild;
			xfree(tmp->data);
			xfree(tmp);
		}
	}
	return *bst;
}

extern void BST_delete_all(BST **bst)
{
	if (*bst == NULL)
		return;
	BST_delete_all(&((*bst)->leftchild));
	BST_delete_all(&((*bst)->rightchild));
	if ((*bst)->leftchild == NULL && (*bst)->rightchild == NULL) {
		xfree((*bst)->data);
		xfree(*bst);
		*bst = NULL;
	}
}

extern void BST_print_all(BST **bst)
{
	if (*bst) {
		BST_print_all(&((*bst)->leftchild));
		//printf("The data is:%s\n", (*bst)->data);
		//printf("The number is%d\n", (*bst)->num);
		BST_print_all(&((*bst)->rightchild));
	}
}
