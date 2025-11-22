#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv) {
	char s[PATH_MAX];
	if (getcwd(s, sizeof(s)) != NULL) {
		printf("%s\n", s);
	} else {
		fprintf(stderr, "getcwd(): Error\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
