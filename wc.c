#include <stdio.h>

/* wc [file]... */

void count(FILE *fp, int *str_cnt, int *word_cnt, int *sym_cnt) {
	int c;
	int in_word = 0;

	while ((c = fgetc(fp)) != EOF) {
		(*sym_cnt)++;
		if (c == '\n')
			(*str_cnt)++;
		if (c == ' ' || c == '\n' || c == '\t')
			in_word = 0;
		else if (in_word == 0) {
			in_word = 1;
			(*word_cnt)++;
		}
	}
}

int main(int argc, char **argv) {
	FILE *fp;
	int str_cnt = 0, word_cnt = 0, sym_cnt = 0;

	if (argc == 1) {
		count(stdin, &str_cnt, &word_cnt, &sym_cnt);
		printf("File: stdin\n");
		printf("	%d %d %d\n", str_cnt, word_cnt, sym_cnt);
	}
	while (--argc) {
		fp = fopen(*++argv, "r");
		count(fp, &str_cnt, &word_cnt, &sym_cnt);
		printf("File: %s\n", *argv);
		printf("	%d %d %d\n", str_cnt, word_cnt, sym_cnt);
		word_cnt = sym_cnt = 0;
		str_cnt = 1;
		fclose(fp);
	}
}
