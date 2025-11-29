#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#define MAX_PATH 1024

#ifndef  MAJOR
  	#define MINOR_BITS 1
  	#define MAJOR(dev) ((unsigned) dev >> MINOR_BITS)
  	#define MINOR(dev) (dev & MINOR_BITS)
#endif

/* ls [-R -l -g] [file]... */

int flag_r;
int flag_l;
int flag_g;

void list_name(char *name);
void display_type (int st_mode);
void display_permission (int st_mode);
void list_all(char *name);
void dir_walk(char *dir, void (*f)(char *));
void list(char *name);

int main(int argc, char **argv) {
	int opt;
	
	while ((opt = getopt(argc, argv, "Rlg")) != -1) {
		switch (opt) {
		case 'R':
			flag_r = 1;
			break;
		case 'l':
			flag_l = 1;
			break;
		case 'g':
			flag_g = 1;
			break;
		default:
			fprintf(stderr, "Usage: ls [-R -l -g] [file]...\n");
			exit(EXIT_FAILURE);
		}
	}

	int cur = optind;
	
	if (cur == argc) {
		list(".");
	} else 	
		while (cur < argc) {
			list(argv[cur]);
			cur++;
		}

	exit(EXIT_SUCCESS);
}

void list_name(char *name) {
	struct stat stbuf;
	struct passwd *pw;
	struct group *gr;
	
	if (lstat(name, &stbuf) == -1) {
		fprintf(stderr, "list_all: can't access %s\n", name);
		return;
	}

	pw = getpwuid(stbuf.st_uid);

	if (flag_g) {
		gr = getgrgid(pw -> pw_gid);
		printf("%s ", gr -> gr_name);
	}

	printf("	%s\n", name);
}

void display_type (int st_mode) {                                   
    switch (st_mode & S_IFMT) {
        case S_IFDIR: putchar ('d'); return;
        case S_IFCHR: putchar ('c'); return;
        case S_IFBLK: putchar ('b'); return;
        case S_IFREG: putchar ('-'); return;
        case S_IFLNK: putchar ('l'); return;
        case S_IFSOCK: putchar ('s'); return;
    }
}

void display_permission (int st_mode) {
  	static const char xtbl[10] = "rwxrwxrwx";
  	char amode[10];
  	int i, j;
 
  	for (i = 0, j = (1 << 8); i < 9; i++, j >>= 1 )
    		amode[i] = (st_mode & j) ? xtbl[i]: '-';
  	if (st_mode & S_ISUID) amode[2]= 's';
  	if (st_mode & S_ISGID) amode[5]= 's';
  	if (st_mode & S_ISVTX) amode[8]= 't';
  	amode[9] = '\0';
  	printf("%s ", amode);
}

void list_all(char *name) {
	struct stat stbuf;
	struct passwd *pw;
	struct group *gr;

	if (lstat(name, &stbuf) == -1) {
		fprintf(stderr, "list_all: can't access %s\n", name);
		return;
	}	

	display_type(stbuf.st_mode);
	display_permission(stbuf.st_mode);
	printf("%ld ", stbuf.st_nlink);
	
	pw = getpwuid(stbuf.st_uid);
	printf("%s ", pw -> pw_name);

	if (flag_g) {
		gr = getgrgid(pw -> pw_gid);
		printf("%s ", gr -> gr_name);
	}

	if (stbuf.st_mode & S_IFMT == S_IFCHR || stbuf.st_mode & S_IFMT == S_IFBLK)
		printf("%d, %ld", MAJOR(stbuf.st_rdev), MINOR(stbuf.st_rdev));
	else
		printf("%8ld ", stbuf.st_size);

	printf("	%s\n", name);
}

void dir_walk(char *dir, void (*f)(char *)) {
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

      if ((dfd = opendir(dir)) == NULL) {
           	fprintf(stderr, "dir_walk: can't open %s\n", dir);
		return; 
	}

	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp -> d_name, ".") == 0 || strcmp(dp -> d_name, "..") == 0)
               	continue;
           	if (strlen(dir) + strlen(dp -> d_name) + 2 > sizeof(name))
               	fprintf(stderr, "dir_walk: name %s %s too long\n", dir, dp -> d_name);
           	else {
               	sprintf(name, "%s/%s", dir, dp -> d_name);
               	(*f)(name);
           	}
	}

	closedir(dfd);
}

void list_rec(char *name) {
	struct stat stbuf;

	if (lstat(name, &stbuf) == -1) {
		fprintf(stderr, "list_rec: can't access %s\n", name);
		return;
	}
	
	printf("%s\n", name);
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dir_walk(name, list_rec);
}

void list_dir(char *dir) {
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

      	if ((dfd = opendir(dir)) == NULL) {
           	fprintf(stderr, "list_dir: can't open %s\n", dir);
		return; 
	}

	while ((dp = readdir(dfd)) != NULL) {
           	if (strlen(dir) + strlen(dp -> d_name) + 2 > sizeof(name))
               		fprintf(stderr, "list_dir: name %s %s too long\n", dir, dp -> d_name);
           	else {
               		sprintf(name, "%s/%s", dir, dp -> d_name);
               		
			if (flag_l)
				list_all(name);
			else
				list_name(name);
           	}
	}

	closedir(dfd);
}

void list(char *name) {
	struct stat stbuf;

      	if (lstat(name, &stbuf) == -1) {
		fprintf(stderr, "list: can't access %s\n", name);
		return; 
	}
	
     	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
		if (flag_r)
			list_rec(name);
		else
			list_dir(name);
	} else if (flag_l)
		list_all(name);
	else
		list_name(name);      
}






