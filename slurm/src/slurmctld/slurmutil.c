/*
 * slurmutil.c
 *
 *  Created on: Jun 11, 2014
 *      Author: kwang
 */

#include <src/slurmctld/slurmutil.h>

extern void c_memset(char *str, int size)
{
	if (!str) {
		str = xmalloc(size);
	}
	else{
		memset(str, '\0', size);
	}
}

extern void xfree_2(char **str, int size)
{
	int i = 0;
	for (; i < size; i++) {
		xfree(str[i]);
	}
	if (str != NULL) {
		xfree(str);
		str = NULL;
	}
}

extern char** xmalloc_2(int first_dim, int second_dim)
{
	char** str = xmalloc(first_dim * sizeof(char*));

	int i;
	for (i = 0; i < first_dim; i++) {
		str[i] = xmalloc(second_dim);
	}

	return str;
}

extern char* int_to_str(int num)
{
	char *str = xmalloc(20);
	sprintf(str, "%d", num);
	return str;
}

extern int str_to_int(char *str)
{
	char **end = NULL;
	int num = (int) (strtol(str, end, 10));
	return num;
}

extern int split_str(char *str, char *delim, char **res)
{
	int count = 0;
	char *pch, *token;

	token = strtok_r(str, delim, &pch);

	while (token != NULL) {
		res[count++] = token;
		token = strtok_r(NULL, delim, &pch);
	}

	return count;
}

extern int find_exist(char **source, char *target, int size)
{
	int count = 0;
	char* tmp = source[count];

	if (tmp == NULL || !strcmp(tmp, "") || !strcmp(tmp, "\0")) {
		return -1;
	}

	while (tmp != NULL && strcmp(tmp, "") && strcmp(tmp, "\0")) {
		if (!strcmp(tmp, target)) {
			break;
		}
		count++;
		if (count >= size) {
			count = -1;
			break;
		}
		tmp = source[count];
	}
	return count;
}
