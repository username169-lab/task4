#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* cat [-n] [file ...] */

int get_sym(int fd) {
	char c;
	
	return (read(fd, &c, 1) == 1) ? (unsigned char) c : EOF;
}

void print(int fd, int flag) {
	int c;
	static int count = 1;
	int newline = 1;

	while ((c = get_sym(fd)) != EOF) {
		if (flag && newline) {
			printf("    %d  ", count);
			newline = 0;
		}
		if (c == '\n') {
			newline = 1;
			count++;
		}
		printf("%c", c);
	}
}

int main(int argc, char **argv) {
	int opt;
	int flag = 0;
	
	while ((opt = getopt(argc, argv, "n")) != -1) {
		switch (opt) {
		case 'n':
			flag = 1;
			break;
		default:
			fprintf(stderr, "Usage: %s [-n] [file ...]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	
	int cur = optind;
	int fd;

	if (cur == argc)
		print(0, flag);

	while (cur < argc) {
		if ((fd = open(argv[cur], O_RDONLY, 0)) == -1)
			fprintf(stderr, "cat: Can't open %s\n", argv[cur]);
		else {
			print(fd, flag);
			close(fd);
		}
		cur++;
	}

 	exit(EXIT_SUCCESS);
}
