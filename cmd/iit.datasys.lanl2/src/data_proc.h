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
	typedef LIST::iterator IT;
	typedef LIST::const_iterator CIT;

public:
	data_proc();
	data_proc(const string& file_all_all);
	virtual ~data_proc();

public:
	void process();

private:

	void init_list(const string& file_all_all);
	string trim(const string& str);

	double process_strict(const string& job_xx_ben_file, int job_scale);
	double process_loose(const string& job_xx_ben_file, int job_scale);

	double process_internal(const string& job_xx_ben_file, int job_scale,
			const bool &loose);
	double process_internal2(const string& job_xx_ben_file, int job_scale,
			const bool &loose);
	double process_internal3(const string& job_xx_ben_file, int job_scale,
			const bool &loose);
	double process_internal4(const string& job_xx_ben_file, int job_scale,
			const bool &loose);
	double process_internal5(const string& job_xx_ben_file, int job_scale,
			const bool &loose);

	int extract_job_scale(const string& job_xx_ben_file);

private:
	LIST file_list;
};
#endif /* DATA_PROC_H_ */
