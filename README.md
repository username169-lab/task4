# task 4

## Implementations of shell commands
echo, pwd, ls, cp, wc, cat

## echo
Usage: echo [-n -s]
int main(int argc, char **argv)

## pwd
Usage: pwd
int main(int argc, char **argv)

## ls
Usage: ls [-R -l -g]
void list_name(char *name)
void display_type(int st_mode)
void display_permission(int st_mode)
void list_all(char *name)
void dir_walk(char *dir, void (*f)(char *))
void list(char *name)
int main(int argc, char **argv) 

## cp
Usage: cp [SOURCE] [DEST]
int main(int argc, char **argv)

## wc
Usage: wc [file]...
void count(FILE *fp, int *str_cnt, int *word_cnt, int *sym_cnt)
int main(int argc, char **argv)

## cat
Usage: cat [-n] [file]...
int get_sym(int fd)
void print(int fd, int flag)
int main(int argc, char **argv)