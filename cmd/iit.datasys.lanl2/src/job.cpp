#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "Util.h"

using namespace std;
using namespace iit::datasys::lanl;

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	double us = 0;
	int printHelp = 0;

	int c;
	while ((c = getopt(argc, argv, "u:h")) != -1) {
		switch (c) {
		case 'u':
			us = atof(optarg);
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

	double start = Util::getTime_usec();

#ifdef LOG_START
	fprintf(stdout, "%f\n", start);
#endif

	usleep(us);

	double end = Util::getTime_usec();

#ifdef LOG_END
	fprintf(stdout, "%f\n", end);
#endif

#ifdef LOG_ALL
//	fprintf(stdout, "sleeped: %f(us)\n", end - start);
	fprintf(stdout, "%f\n", start);
	fprintf(stdout, "%f\n", end);
#endif

}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0, "[-u microseconds] [-h(help)]");
}

