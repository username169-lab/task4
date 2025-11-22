#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdio.h>
 
#ifndef  MAJOR
	#define MINOR_BITS
 	#define MAJOR(dev) ((unsigned) dev >> MINOR_BITS)
  	#define MINOR(dev) (dev & MINOR_BITS)
#endif
 
void display_file_type (int st_mode) {                                   
    	switch (st_mode & S_IFMT) {
        case S_IFDIR:  putchar ('d'); return;
        case S_IFCHR:  putchar ('c'); return;
        case S_IFBLK:  putchar ('b'); return;
        case S_IFREG:  putchar ('-'); return;
        case S_IFLNK:  putchar ('l'); return;
        case S_IFSOCK: putchar ('s'); return;
    }
} 
 
void display_permission ( int st_mode )	{
  	static const char xtbl[10] = "rwxrwxrwx";
  	char amode[10];
  	int  i, j;
 
	for ( i = 0, j = ( 1 << 8 ); i < 9; i++, j >>= 1 )
    		amode[i] = ( st_mode&j ) ? xtbl[i]: '-';
  	if ( st_mode & S_ISUID )   amode[2]= 's';
  	if ( st_mode & S_ISGID )   amode[5]= 's';
  	if ( st_mode & S_ISVTX )   amode[8]= 't';
  	amode[9]='\0';
  	printf ( "%s ",amode );
}
 
void long_list ( char * path_name ) {
  	struct stat     statv;
  	struct passwd  *pw_d;
 
      	if ( lstat ( path_name, &statv ) ) { 
    	perror ( path_name ); 
    	return;
	}
  	display_file_type ( statv.st_mode );
  	display_permission ( statv.st_mode );
  	printf ( "%d ",statv.st_nlink );
  	pw_d = getpwuid ( statv.st_uid );
  	printf ( "%s ",pw_d->pw_name );
 
	if (
      ( statv.st_mode & S_IFMT) == S_IFCHR  ||
      ( statv.st_mode & S_IFMT) == S_IFBLK
     )
    		printf ( "%d, %d",MAJOR(statv.st_rdev), MINOR(statv.st_rdev) );
  	else
   		printf ( "%d", statv.st_size );
  	printf ( "     %s\n", path_name );
}
 
int main (int argc, char **argv) {
  	if (argc == 1)
    		fprintf(stderr, "usage: %s <path name> ...\n", argv[0]);
  	else
   	 	while (argc-- != 1) 
			long_list(*++argv);
	return 0;
}
