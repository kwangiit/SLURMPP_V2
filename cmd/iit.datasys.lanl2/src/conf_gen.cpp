#include "conf_gen.h"
#include "String_Tokenizer.h"
#include "str_trim.h"
#include <fstream>

typedef String_Tokenizer str_tok;

conf_gen::conf_gen() {
}

conf_gen::conf_gen(const string& expr_name, const string& conf_file,
		const string& host_file, int start, int end, const string& ctlmachine,
		const string& ctladdr) :
		_expr_name(expr_name), _conf_file(conf_file), _host_file(host_file), _start(
				start), _end(end), _ctlmachine(ctlmachine), _ctladdr(ctladdr) {
}

conf_gen::~conf_gen() {
}

string conf_gen::to_string() {

	string conf;

	CIT it;

	for (it = _conf_list.begin(); it != _conf_list.end(); it++) {

		conf.append(*it);
		conf.append("\n");
	}

	for (it = _spec_list.begin(); it != _spec_list.end(); it++) {

		conf.append(*it);
		conf.append(" ");
	}

	conf.append("\n");

	return conf;
}

string conf_gen::gen_node_arr_list() {

	string prefix = "ko";

	string middle_part = "";

	string host_line;
	ifstream ifs(_host_file);

	while (getline(ifs, host_line)) {

		string line = trim(host_line);
		if (line == "")
			continue;

		str_tok tokens(line); // " ", delimiter
		string hostname = tokens.next_token();

		middle_part.append(hostname.substr(2)); //"0001,0002,0003"
		middle_part.append(","); //"0001,0002,0003,"
	}

	string list;

	list.append(prefix);
	list.append("[");
	middle_part = middle_part.substr(0, middle_part.size() - 1); //remove last ,
	list.append(middle_part);
	list.append("]");

	return list;
}

string conf_gen::replace_at_tok(const string& line) {

	string result;

	string at_tok = "expr";

	str_tok tokens(line, "@"); // " ", delimiter

	while (tokens.has_more_tokens()) {

		string token = trim(tokens.next_token());

		if (token == at_tok)
			result.append(_expr_name);
		else
			result.append(token);
	}

	if (result.empty())
		return line;
	else
		return result;
}

string conf_gen::gen_conf() {

	string spec_line = "# COMPUTE NODES";
	string ctlmachine_line = "ControlMachine=##";
	string ctladdr_line = "ControlAddr=##";

	bool spec_line_hit = false;

	string conf_line;
	ifstream ifs(_conf_file);

	while (getline(ifs, conf_line)) {

//		string line = replace_at_tok(conf_line);
		string line = conf_line;

		if (line == spec_line) { //"# COMPUTE NODES";

			spec_line_hit = true;
			_conf_list.push_back(line);

		} else if (line == ctlmachine_line) { //ControlMachine=##

			char buf[100];
			sprintf(buf, "ControlMachine=%s", _ctlmachine.c_str());

			string tmp(buf);
			_conf_list.push_back(tmp);

		} else if (line == ctladdr_line) { //ControlAddr=##

			char buf[100];
			sprintf(buf, "ControlAddr=%s", _ctladdr.c_str());

			string tmp(buf);
			_conf_list.push_back(tmp);
		} else {

			if (spec_line_hit) {

				_store_list.push_back(line);
				//spec_line_hit = false;

			} else {

				_conf_list.push_back(line);
			}
		}
	}

	/*NodeName=node-[3-4] Nodeaddr=node-[3-4].xiaobing.usrc CPUs=2 Sockets=2 CoresPerSocket=1 ThreadsPerCore=1 State=UNKNOWN*/
	for (IT it = _store_list.begin(); it != _store_list.end(); it++) {

		str_tok attrs(*it); // " ", delimiter

		bool nodes_hit = false;
		bool nodename_hit = false;
		bool nodeaddr_hit = false;

		string nodes = "Nodes"; //Nodes=node-[2-101]
		string nodename = "NodeName"; //NodeName=node-[2-101]
		string nodeaddr = "Nodeaddr"; //Nodeaddr=node-[3-4].xiaobing.usrc

		while (attrs.has_more_tokens()) {

			string attr = trim(attrs.next_token()); //Nodeaddr=node-[3-4].xiaobing.usrc

			str_tok name_value(attr, "=");
			string name = name_value.next_token(); //e.g. Nodeaddr

			if (name == nodes) { //"Nodes"
				nodes_hit = true;
			} else if (name == nodename) { //"NodeName"
				nodename_hit = true;
			} else if (name == nodeaddr) { //"Nodeaddr"
				nodeaddr_hit = true;
			} else
				;

			if (nodes_hit) {

				char buf[100];
				sprintf(buf, "Nodes=node-[%d-%d]", _start, _end);

				string tmp(buf);
				_spec_list.push_back(tmp);

				nodes_hit = false;

			} else if (nodename_hit) {

				char buf[100];
				sprintf(buf, "NodeName=node-[%d-%d]", _start, _end);
				string tmp(buf);
				_spec_list.push_back(tmp);

				nodename_hit = false;

			} else if (nodeaddr_hit) {

				string node_addr_list = gen_node_arr_list();

				char buf[10000];
				sprintf(buf, "Nodeaddr=%s", node_addr_list.c_str());
				string tmp(buf);
				_spec_list.push_back(tmp);

				nodeaddr_hit = false;

			} else {
				_spec_list.push_back(attr);
			}
		}

		_spec_list.push_back("\n");
	}

	return to_string();
}

string conf_gen::trim(const string& str) {

	return str_trim::trim(str);
}

#include <iostream>
#include <getopt.h>

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	double us = 0;
	int printHelp = 0;

	string expr_name;
	string conf_file;
	string host_file;
	string save_file;

	int start = 0;
	int end = 0;

	string ctlmachine;
	string ctladdr;

	int c;
	while ((c = getopt(argc, argv, "n:c:t:s:a:d:m:r:h")) != -1) {
		switch (c) {
		case 'n':
			expr_name = optarg;
			break;
		case 'c':
			conf_file = optarg;
			break;
		case 't':
			host_file = optarg;
			break;
		case 's':
			save_file = optarg;
			break;
		case 'a':
			start = atoi(optarg);
			break;
		case 'd':
			end = atoi(optarg);
			break;
		case 'm':
			ctlmachine = optarg;
			break;
		case 'r':
			ctladdr = optarg;
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

	if (!conf_file.empty() && !host_file.empty() && !save_file.empty() && start
			&& end && !ctlmachine.empty() && !ctladdr.empty()) {

		conf_gen cg(expr_name, conf_file, host_file, start, end, ctlmachine,
				ctladdr);

		ofstream ofs(save_file);
		ofs << cg.gen_conf();

	} else {

		printUsage(argv[0]);
		exit(1);
	}

	return 0;
}

void printUsage(char *argv_0) {

	//start end
	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-c conf_file -t slave_hosts_file -s save_file -a start -d end -m ctlmachine -r ctladdress [-h(help)]");
}

