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

data_proc::data_proc() {
}

data_proc::data_proc(const string& filename_list_file) {

	init_list(filename_list_file);
}

data_proc::~data_proc() {
}

string data_proc::trim(const string& str) {

	return str_trim::trim(str);
}

void data_proc::init_list(const string& filename_list_file) {

	string line;

	ifstream ifs(filename_list_file);

	while (getline(ifs, line)) {

		string file_name = trim(line); //task.512.out, task.256.out...

		if (file_name.empty())
			continue;

		file_list.push_back(file_name);
	}
}

/*
 * @filename, like task.512.out
 * */
double data_proc::process_internal(const string& filename, int node_scale) {

	string line;
	double time_start;

	ifstream ifs(filename);

	vector<double> vec;

	int i = 0;
	while (getline(ifs, line)) {

		if (i == 0) {

			time_start = atof(line.c_str());
		} else {

			String_Tokenizer tokens(line); //line looks like: "112: 1365820275826305.000000"
			tokens.next_token();
			string time = tokens.next_token(); //get "1365820275826305.000000"

			vec.push_back(atof(time.c_str()));
		}

		i++;
	}

	/*task.512.out ==> i = 1024, since two tasks run on every node, plus the entry of start time*/
	if (i != (2 * node_scale + 1)) {

		char buf[200];
		sprintf(buf, "task.%d: only %d of %d ran.", node_scale, i, node_scale);

		throw logic_error(buf);
	}

	sort(vec.begin(), vec.end());

	double time_end = vec[vec.size() - 1];
	double us = time_end - time_start;

	return us;
}

/*
 * @filename, like task.512.out
 * */
string data_proc::extract_node_scale(const string& filename) {

	string node_scale;

	String_Tokenizer tokens(filename, "."); //task.512.out

	int i = 0;
	while (tokens.has_more_tokens()) {

		string token = tokens.next_token();

		if (i == 1) { //get 512 from task.512.out

			node_scale = token;
			break;
		}

		i++;
	}

	return node_scale;
}

void data_proc::process(const string& agg_out_file, const string& avg_out_file,
		const string& tp_out_file) {

	ofstream agg_ofs(agg_out_file);
	ofstream avg_ofs(avg_out_file);
	ofstream tp_ofs(tp_out_file);

	CIT it;

	for (it = file_list.begin(); it != file_list.end(); it++) {

		string filename = *it;

		string node_scale = extract_node_scale(filename);

		double agg = process_internal(filename, atoi(node_scale.c_str()));

		double avg = agg / (2 * atoi(node_scale.c_str()));

		double throughput = 1.0 * 1000000 / avg;

		char buf[100];
		sprintf(buf, "%f", agg);

		agg_ofs << node_scale << "\t" << buf << "\n";

		memset(buf, 0, 100);
		sprintf(buf, "%f", avg);

		avg_ofs << node_scale << "\t" << buf << "\n";

		memset(buf, 0, 100);
		sprintf(buf, "%f", throughput);

		tp_ofs << node_scale << "\t" << buf << "\n";

	}
}

#include <getopt.h>

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	string filename_list_file;
	string agg_out_file = "task.agg.ben";
	string avg_out_file = "task.avg.ben";
	string tp_out_file = "task.tp.ben";

	int printHelp = 0;

	int c;
	while ((c = getopt(argc, argv, "l:g:a:t:h")) != -1) {
		switch (c) {
		case 'l':
			filename_list_file = optarg;
			break;
		case 'g':
			agg_out_file = optarg;
			break;
		case 'a':
			avg_out_file = optarg;
			break;
		case 't':
			tp_out_file = optarg;
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

	if (!filename_list_file.empty()) {

		data_proc proc(filename_list_file);

		proc.process(agg_out_file, avg_out_file, tp_out_file);

	} else {

		printUsage(argv[0]);
		exit(1);
	}
}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-l filename_list_file [-g agg_out_file] [-a avg_out_file] [-h(help)]");
}
