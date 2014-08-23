#include "data_proc.h"

#include "str_trim.h"

#include "String_Tokenizer.h"

#include "Util.h"

#include <fstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <stdexcept>
#include <string.h>
#include <stdio.h>

data_proc::data_proc() {
}

data_proc::data_proc(const string& file_all_all) {

	init_list(file_all_all);
}

data_proc::~data_proc() {
}

string data_proc::trim(const string& str) {

	return str_trim::trim(str);
}

/*
 * extract file entry from file_all_all, the output like:
 * job.2.ben
 * job.4.ben
 * job.8.ben
 * job.16.ben
 * job.32.ben
 * ......
 *
 * */
void data_proc::init_list(const string& file_all_all) {

	string line;

	ifstream ifs(file_all_all);

	while (getline(ifs, line)) {

		string file_name = trim(line); //task.512.out, task.256.out...

		if (file_name.empty())
			continue;

		file_list.push_back(file_name);
	}
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_strict(const string& job_xx_ben_file, int job_scale) {

	double timespan_sum;

#ifdef WAY1
	timespan_sum = process_internal(job_xx_ben_file, job_scale, false);
#elif WAY2
	timespan_sum = process_internal2(job_xx_ben_file, job_scale, false);
#elif WAY3
	timespan_sum = process_internal3(job_xx_ben_file, job_scale, false);
#elif WAY4
	timespan_sum = process_internal4(job_xx_ben_file, job_scale, false);
#elif WAY5
	timespan_sum = process_internal5(job_xx_ben_file, job_scale, false);
#endif

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_loose(const string& job_xx_ben_file, int job_scale) {

	double timespan_sum;

#ifdef WAY1
	timespan_sum = process_internal(job_xx_ben_file, job_scale, true);
#elif WAY2
	timespan_sum = process_internal2(job_xx_ben_file, job_scale, true);
#elif WAY3
	timespan_sum = process_internal3(job_xx_ben_file, job_scale, true);
#elif WAY4
	timespan_sum = process_internal4(job_xx_ben_file, job_scale, true);
#elif WAY5
	timespan_sum = process_internal5(job_xx_ben_file, job_scale, true);
#endif

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_internal4(const string& job_xx_ben_file,
		int job_scale, const bool &loose) {

	double timespan_sum = 0;
	string job_xx_out_file;
	double time_start = 0;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	/*time_start/time_end print-out, for 1024 parallel jobs on 1024 cores of 512 nodes*/
	int num_lines = 2048;

	int i = 0;
	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		fprintf(stdout, "[%s][%s]: processing...\n", job_xx_ben_file.c_str(),
				job_xx_out_file.c_str());

		ifstream job_xx_out_ifs(job_xx_out_file);

		/*the first line is job.x.0.out, count the timestamp in it as time start*/
		if (loose && i == 0) {

			string timestamp_line;

			while (getline(job_xx_out_ifs, timestamp_line)) {

				time_start = atof(timestamp_line.c_str());
				break; /*since there's only one line in job.x.0.out*/
			}

		}

		if (i > 0) { /*count the earliest timestamp in all job.x.x.out(except job.x.0.out) as time start*/

			vector<double> vec_start;
			vector<double> vec_end;

			/* the format, [MPI process number]: [timestamp]
			 0163: 1366949424667582.000000
			 0163: 1366949424667658.000000
			 * */
			string timestamp_line;

			int j = 0; //counter for lines of timestamp in job.xx.out file
			int first_proc_num; //the first MPI processor number
			int second_proc_num; //the second MPI processor number

			while (getline(job_xx_out_ifs, timestamp_line)) {

				j++;

				if (timestamp_line.empty())
					continue;

				String_Tokenizer tokens(timestamp_line, ":"); //
				string proc_num = trim(tokens.next_token()); //0163, the MPI process number
				string timestamp = trim(tokens.next_token()); //1366949424667582.000000, the timestamp

				if (j % 2 != 0)
					first_proc_num = atoi(proc_num.c_str());
				else
					second_proc_num = atoi(proc_num.c_str());

				if (j % 2 == 0) {

					if (first_proc_num != second_proc_num) {

						char buf[200];
						sprintf(buf,
								"[%s][%d][%d]: the timestamp of start and end is not contiguous.",
								job_xx_out_file.c_str(), first_proc_num,
								second_proc_num);

						throw logic_error(buf);
					}
				}

				if (j % 2 != 0)
					vec_start.push_back(atof(timestamp.c_str()));
				else
					vec_end.push_back(atof(timestamp.c_str()));
			}

			if (j != num_lines) {

				throw logic_error(
						"less or more than 2048 timestamp lines in job.xx.out file.");
			}

			sort(vec_start.begin(), vec_start.end());
			sort(vec_end.begin(), vec_end.end());

			if (!loose)
				time_start = vec_start[0];

			double timespan = vec_start[vec_start.size() - 1] - time_start;

			timespan_sum += timespan;
		}

		i++;
	}

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_internal5(const string& job_xx_ben_file,
		int job_scale, const bool &loose) {

	string job_xx_out_file;
	double time_start = 0;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	/*lines of time_start/time_end print-out*/
	int num_lines = 2;

	vector<double> vec_start;
	vector<double> vec_end;

	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		fprintf(stdout, "[%s][%s]: processing...\n", job_xx_ben_file.c_str(),
				job_xx_out_file.c_str());

		ifstream job_xx_out_ifs(job_xx_out_file);

		/* the format, [jobid]: [timestamp]
		 78: 1366949424667582.000000
		 78: 1366949424667658.000000
		 * */
		string timestamp_line;

		int j = 0; //counter for lines of timestamp in job.xx.out file
		int first_jobid; //the first jobid
		int second_jobid; //the second jobid

		while (getline(job_xx_out_ifs, timestamp_line)) {

			j++;

			if (timestamp_line.empty())
				continue;

			String_Tokenizer tokens(timestamp_line, ":"); //
			string jobid = trim(tokens.next_token()); //78, the jobid
			string timestamp = trim(tokens.next_token()); //1366949424667582.000000, the timestamp

			if (j % 2 != 0)
				first_jobid = atoi(jobid.c_str());
			else
				second_jobid = atoi(jobid.c_str());

			if (j % 2 == 0) {

				if (first_jobid != second_jobid) {

					char buf[200];
					sprintf(buf,
							"[%s][%d][%d]: the timestamp of start and end is not contiguous.",
							job_xx_out_file.c_str(), first_jobid, second_jobid);

					throw logic_error(buf);
				}
			}

			if (j % 2 != 0)
				vec_start.push_back(atof(timestamp.c_str()));
			else
				vec_end.push_back(atof(timestamp.c_str()));
		}

		if (j != num_lines) {

			char buf[200];
			sprintf(buf,
					"[%s][%d][%d]: less or more than %d timestamp lines in job.xx.out file.",
					job_xx_out_file.c_str(), first_jobid, second_jobid,
					num_lines);

			throw logic_error(buf);
		}
	}

	sort(vec_start.begin(), vec_start.end());
	sort(vec_end.begin(), vec_end.end());

	double timespan_sum = vec_end[vec_end.size() - 1] - vec_start[0];

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_internal3(const string& job_xx_ben_file,
		int job_scale, const bool &loose) {

	double timespan_sum = 0;
	string job_xx_out_file;
	double time_start = 0;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	/*time_start/time_end print-out, for 1024 parallel jobs on 1024 cores of 512 nodes*/
	int num_lines = 2048;

	int i = 0;
	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		fprintf(stdout, "[%s][%s]: processing...\n", job_xx_ben_file.c_str(),
				job_xx_out_file.c_str());

		ifstream job_xx_out_ifs(job_xx_out_file);

		/*the first line is job.x.0.out, count the timestamp in it as time start*/
		if (loose && i == 0) {

			string timestamp_line;

			while (getline(job_xx_out_ifs, timestamp_line)) {

				time_start = atof(timestamp_line.c_str());
				break; /*since there's only one line in job.x.0.out*/
			}

		}

		if (i > 0) { /*count the earliest timestamp in all job.x.x.out(except job.x.0.out) as time start*/

			/* the format, [MPI process number]: [timestamp]
			 0163: 1366949424667582.000000
			 0163: 1366949424667658.000000
			 * */
			string timestamp_line;

			int j = 0; //counter for lines of timestamp in job.xx.out file
			int first_proc_num; //the first MPI processor number
			int second_proc_num; //the second MPI processor number

			double pair_start_time = 0; //the start time of allone
			double pair_end_time = 0; //the end time of allone

			while (getline(job_xx_out_ifs, timestamp_line)) {

				j++;

				if (timestamp_line.empty())
					continue;

				String_Tokenizer tokens(timestamp_line, ":"); //
				string proc_num = trim(tokens.next_token()); //0163, the MPI process number
				string timestamp = trim(tokens.next_token()); //1366949424667582.000000, the timestamp

				if (j % 2 != 0) {

					first_proc_num = atoi(proc_num.c_str());
					pair_start_time = atof(timestamp.c_str());
				} else {

					second_proc_num = atoi(proc_num.c_str());
					pair_end_time = atof(timestamp.c_str());
				}

				if (j % 2 == 0) {

					if (first_proc_num != second_proc_num) {

						char buf[200];
						sprintf(buf,
								"[%s][%d][%d]: the timestamp of start and end is not contiguous.",
								job_xx_out_file.c_str(), first_proc_num,
								second_proc_num);

						throw logic_error(buf);
					}
				}

				if (j % 2 == 0)
					timespan_sum += pair_end_time - pair_start_time;
			}

			if (j != num_lines) {

				throw logic_error(
						"less or more than 2048 timestamp lines in job.xx.out file.");
			}
		}

		i++;
	}

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_internal2(const string& job_xx_ben_file,
		int job_scale, const bool &loose) {

	double timespan_sum = 0;
	string job_xx_out_file;
	double time_start = 0;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	/*time_start/time_end print-out, for 1024 parallel jobs on 1024 cores of 512 nodes*/
	int num_lines = 2048;

	int i = 0;
	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		fprintf(stdout, "[%s][%s]: processing...\n", job_xx_ben_file.c_str(),
				job_xx_out_file.c_str());

		ifstream job_xx_out_ifs(job_xx_out_file);

		/*the first line is job.x.0.out, count the timestamp in it as time start*/
		if (loose && i == 0) {

			string timestamp_line;

			while (getline(job_xx_out_ifs, timestamp_line)) {

				time_start = atof(timestamp_line.c_str());
				break; /*since there's only one line in job.x.0.out*/
			}

		}

		if (i > 0) { /*count the earliest timestamp in all job.x.x.out(except job.x.0.out) as time start*/

			vector<double> vec_start;
			vector<double> vec_end;

			/* the format, [MPI process number]: [timestamp]
			 0163: 1366949424667582.000000
			 0163: 1366949424667658.000000
			 * */
			string timestamp_line;

			int j = 0; //counter for lines of timestamp in job.xx.out file
			int first_proc_num; //the first MPI processor number
			int second_proc_num; //the second MPI processor number

			while (getline(job_xx_out_ifs, timestamp_line)) {

				j++;

				if (timestamp_line.empty())
					continue;

				String_Tokenizer tokens(timestamp_line, ":"); //
				string proc_num = trim(tokens.next_token()); //0163, the MPI process number
				string timestamp = trim(tokens.next_token()); //1366949424667582.000000, the timestamp

				if (j % 2 != 0)
					first_proc_num = atoi(proc_num.c_str());
				else
					second_proc_num = atoi(proc_num.c_str());

				if (j % 2 == 0) {

					if (first_proc_num != second_proc_num) {

						char buf[200];
						sprintf(buf,
								"[%s][%d][%d]: the timestamp of start and end is not contiguous.",
								job_xx_out_file.c_str(), first_proc_num,
								second_proc_num);

						throw logic_error(buf);
					}
				}

				if (j % 2 != 0)
					vec_start.push_back(atof(timestamp.c_str()));
				else
					vec_end.push_back(atof(timestamp.c_str()));
			}

			if (j != num_lines) {

				throw logic_error(
						"less or more than 2048 timestamp lines in job.xx.out file.");
			}

			sort(vec_start.begin(), vec_start.end());
			sort(vec_end.begin(), vec_end.end());

			double timespan = vec_end[vec_end.size() - 1] - vec_start[0];

			timespan_sum += timespan;
		}

		i++;
	}

	return timespan_sum;
}

/*
 * @job_xx_ben_file, like job.4.ben, it contains
 job.4.0.out
 job.4.1.out
 job.4.2.out
 job.4.3.out
 job.4.4.out
 *
 * */
double data_proc::process_internal(const string& job_xx_ben_file, int job_scale,
		const bool &loose) {

	string job_xx_out_file;
	double time_start = 0;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	/*lines of time_start/time_end print-out, for 1024 parallel jobs on 1024 cores of 512 nodes*/
	int num_lines = 2048;

	vector<double> vec_start;
	vector<double> vec_end;

	int i = 0;
	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		fprintf(stdout, "[%s][%s]: processing...\n", job_xx_ben_file.c_str(),
				job_xx_out_file.c_str());

		ifstream job_xx_out_ifs(job_xx_out_file);

		/*the first line is job.x.0.out, count the timestamp in it as time start*/
		if (loose && i == 0) {

			string timestamp_line;

			while (getline(job_xx_out_ifs, timestamp_line)) {

				time_start = atof(timestamp_line.c_str());
				break; /*since there's only one line in job.x.0.out*/
			}

		}

		if (i > 0) { /*count the earliest timestamp in all job.x.x.out(except job.x.0.out) as time start*/

			/* the format, [MPI process number]: [timestamp]
			 0163: 1366949424667582.000000
			 0163: 1366949424667658.000000
			 * */
			string timestamp_line;

			int j = 0; //counter for lines of timestamp in job.xx.out file
			int first_proc_num; //the first MPI processor number
			int second_proc_num; //the second MPI processor number

			while (getline(job_xx_out_ifs, timestamp_line)) {

				j++;

				if (timestamp_line.empty())
					continue;

				String_Tokenizer tokens(timestamp_line, ":"); //
				string proc_num = trim(tokens.next_token()); //0163, the MPI process number
				string timestamp = trim(tokens.next_token()); //1366949424667582.000000, the timestamp

				if (j % 2 != 0)
					first_proc_num = atoi(proc_num.c_str());
				else
					second_proc_num = atoi(proc_num.c_str());

				if (j % 2 == 0) {

					if (first_proc_num != second_proc_num) {

						char buf[200];
						sprintf(buf,
								"[%s][%d][%d]: the timestamp of start and end is not contiguous.",
								job_xx_out_file.c_str(), first_proc_num,
								second_proc_num);

						throw logic_error(buf);
					}
				}

				if (j % 2 != 0)
					vec_start.push_back(atof(timestamp.c_str()));
				else
					vec_end.push_back(atof(timestamp.c_str()));
			}

			if (j != num_lines) {

				throw logic_error(
						"less or more than 2048 timestamp lines in job.xx.out file.");
			}
		}

		i++;
	}

	sort(vec_start.begin(), vec_start.end());
	sort(vec_end.begin(), vec_end.end());

	if (!loose)
		time_start = vec_start[0];

	double timespan_sum = vec_end[vec_end.size() - 1] - time_start;

	return timespan_sum;
}

/*
 * @filename, like job.512.ben
 * */
int data_proc::extract_job_scale(const string& job_xx_ben_file) {

	string job_xx_out_file;

	ifstream job_xx_ben_ifs(job_xx_ben_file);

	int i = 0;
	while (getline(job_xx_ben_ifs, job_xx_out_file)) {

		i++;
	}

	return i;
}

/*
 * @filename, like job.512.ben
 * */
string extract_job_scale2(const string& job_xx_ben_file) {

	string job_scale;

	String_Tokenizer tokens(job_xx_ben_file, "."); //job.512.ben

	int i = 0;
	while (tokens.has_more_tokens()) {

		string token = tokens.next_token();

		if (i == 1) { //get 512 from job.512.ben

			job_scale = token;
			break;
		}

		i++;
	}

	return job_scale;
}

/*
 * file_list's content:
 * job.2.ben
 * job.4.ben
 * job.8.ben
 * job.16.ben
 * job.32.ben
 * ......
 *
 * */
void data_proc::process() {

	string job_agg_rpr = "job.agg.rpr";
	string job_avg_rpr = "job.avg.rpr";
	string job_thp_rpr = "job.thp.rpr";

	string job_agg2_rpr = "job.agg2.rpr";
	string job_avg2_rpr = "job.avg2.rpr";
	string job_thp2_rpr = "job.thp2.rpr";

	ofstream agg_ofs(job_agg_rpr);
	ofstream avg_ofs(job_avg_rpr);
	ofstream thp_ofs(job_thp_rpr);

	ofstream agg2_ofs(job_agg2_rpr);
	ofstream avg2_ofs(job_avg2_rpr);
	ofstream thp2_ofs(job_thp2_rpr);

	CIT it;

	for (it = file_list.begin(); it != file_list.end(); it++) {

		string job_xx_ben_file = *it;

		int job_scale = extract_job_scale(job_xx_ben_file);

		double agg = process_strict(job_xx_ben_file, job_scale);
		agg /= 1000.0;
		double avg = agg / job_scale;
		double thp = 1.0 * 1000 / avg;

		double agg2 = process_loose(job_xx_ben_file, job_scale);
		agg2 /= 1000.0;
		double avg2 = agg2 / job_scale;
		double thp2 = 1.0 * 1000 / avg2;

		char buf[100];

		/*memorize aggregation time for strict time computation*/
		sprintf(buf, "%f", agg);
		agg_ofs << job_scale << "\t" << buf << "\n";

		/*memorize average time for strict time computation*/
		memset(buf, 0, 100);
		sprintf(buf, "%f", avg);
		avg_ofs << job_scale << "\t" << buf << "\n";

		/*memorize throughput for strict time computation*/
		memset(buf, 0, 100);
		sprintf(buf, "%f", thp);
		thp_ofs << job_scale << "\t" << buf << "\n";

		/*memorize aggregation time for loose time computation*/
		sprintf(buf, "%f", agg2);
		agg2_ofs << job_scale << "\t" << buf << "\n";

		/*memorize average time for loose time computation*/
		memset(buf, 0, 100);
		sprintf(buf, "%f", avg2);
		avg2_ofs << job_scale << "\t" << buf << "\n";

		/*memorize throughput time for loose time computation*/
		memset(buf, 0, 100);
		sprintf(buf, "%f", thp2);
		thp2_ofs << job_scale << "\t" << buf << "\n";

	}

}

/*
 * file hierarchy
 * job.all.all
 * 		job.2.ben -> time end - time start
 * 			job.2.0.out
 * 			job.2.1.out
 * 			job.2.2.out
 * 		job.4.ben -> time end - time start
 * 			job.4.0.out
 * 			job.4.1.out
 * 			job.4.2.out
 * 			job.4.3.out
 * 			job.4.4.out
 * 		....
 *
 *		job.agg.rpr
 *		job.avg.rpr
 *		job.thp.rpr
 *
 *		job.agg2.rpr
 *		job.avg2.rpr
 *		job.thp2.rpr
 *
 * */

#include <getopt.h>

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	string file_all_all;

	int printHelp = 0;

	int c;
	while ((c = getopt(argc, argv, "f:h")) != -1) {

		switch (c) {
		case 'f':
			file_all_all = optarg;
			break;
		case 'h':
			printHelp = 1;
			break;
		default:
			fprintf(stderr, "Illegal argument \"%c\"\n", c);
			printUsage(argv[0]);
			exit(1);
		}
	}

	if (printHelp) {
		printUsage(argv[0]);
		exit(1);
	}

	if (!file_all_all.empty()) {

		data_proc proc(file_all_all);

		proc.process();

	} else {

		printUsage(argv[0]);
		exit(1);
	}
}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0, "-f file_all_all [-h(help)]");
}
