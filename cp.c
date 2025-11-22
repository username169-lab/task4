#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define PERMS 0666

/* cp SOURCE DEST */
int main(int argc, char **argv) {
    int fs, fd, n;
    int buf[BUFSIZ];
    struct stat perms;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: cp SOURCE DEST\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], argv[2]) != 0) {
        if ((fs = open(argv[1], O_RDONLY, 0)) == -1) {
            fprintf(stderr, "cp: Can't open %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        stat(argv[1], &perms);
        if ((fd = creat(argv[2], PERMS)) == -1) {
            fprintf(stderr, "cp: Can't create %s\n, mode %03o", argv[2], PERMS);
            close(fs);
            exit(EXIT_FAILURE);
        }
        while ((n = read(fs, buf, BUFSIZ)) > 0)
            if (write(fd, buf, n) != n) {
                fprintf(stderr, "cp: Write error on file %s\n", argv[2]);
                close(fs);
                close(fd);
                exit(EXIT_FAILURE);
            }
        fchmod(fd, perms.st_mode);
        close(fs);
        close(fd);
    } else {
        printf("cp: Error same file\n");
    }
    
    exit(EXIT_SUCCESS);
}
