/*
 * data_proc.h
 *
 *  Created on: Apr 13, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef DATA_PROC_H_
#define DATA_PROC_H_

#include <list>
#include <string>
using namespace std;

class data_proc {

public:
	typedef list<string> LIST;
	typedef typename LIST::iterator IT;
	typedef typename LIST::const_iterator CIT;

public:
	data_proc();
	data_proc(const string& filename_list_file);
	virtual ~data_proc();

public:
	void process(const string& agg_out_file, const string& avg_out_file,
			const string& tp_out_file);

private:

	void init_list(const string& filename_list_file);
	string trim(const string& str);

	double process_internal(const string& filename, int node_scale);

	string extract_node_scale(const string& filename);

private:
	LIST file_list;
};
#endif /* DATA_PROC_H_ */
