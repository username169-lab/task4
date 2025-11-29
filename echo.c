#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	int opt;
	int flag_n = 0, flag_s = 0;

	while ((opt = getopt (argc, argv, "ns")) != -1) {
		switch (opt) {
		case 'n':
			flag_n = 1;
			break;
		case 's':
			flag_s = 1;
			break;
		default:
			fprintf(stderr, "Usage: echo [-n -s] [file ...]\n");
			exit(EXIT_FAILURE);
		}
	}

	int cur = optind;
	while (cur < argc) {
		if (cur == argc - 1)
			printf(flag_n ? "%s" : "%s\n", argv[cur]);
		else
			printf(flag_s ? "%s" : "%s ", argv[cur]);
		cur++;
	}

	exit(EXIT_SUCCESS);
}
