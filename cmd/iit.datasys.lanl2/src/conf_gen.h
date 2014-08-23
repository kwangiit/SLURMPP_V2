#ifndef CONF_GEN_H
#define CONF_GEN_H

#include <string>
#include <list>

using namespace std;

class conf_gen {

public:
	typedef list<string> LIST;
	typedef LIST::iterator IT;
	typedef LIST::const_iterator CIT;

public:
	conf_gen();
	conf_gen(const string& expr_name, const string& conf_file,
			const string& host_file, int start, int end,
			const string& ctlmachine, const string& ctladdr);
	virtual ~conf_gen();

public:

	string gen_conf();

private:

	string trim(const string& str);

	string to_string();

	string gen_node_arr_list();

	string replace_at_tok(const string& line);

private:
	string _expr_name;
	string _conf_file;
	string _host_file;
	string _ctlmachine;
	string _ctladdr;
	int _start;
	int _end;

	LIST _conf_list;
	LIST _spec_list;
	LIST _store_list;
};

#endif
