
 #include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>


int ls (char * direct) {
	DIR *dir;
	dir = opendir(direct);
	if (!dir) {
		perror ("unable to open directory");
	}
	struct dirent *file;
	while ((file = readdir (dir)) != 0) {
		if ((strcmp(file->d_name, ".'") && strcmp (file->d_name, "..")))
			printf ("%s\n", file->d_name);
	}
	closedir (dir); //proverka na -1 ?
	return 0;
}

int ls_l (char *direct) {
	DIR *dir;
	dir = opendir (direct);
	struct dirent *file;
	while ((file = readdir(dir)) != 0) {
		if (strcmp (file->d_name, ".") && strcmp (file->d_name, "..")) {
			struct stat buf;
			stat (file->d_name, &buf);
			int mode = buf.st_mode;
			if (!(mode & S_IRUSR)){
			fprintf(stderr, "can't read Directory\n");
			continue;
		}
		if (mode & S_IFDIR) printf("d");
		else if (mode & S_IFBLK) printf("b");
		else if (mode & S_IFCHR) printf("c");
		else if (mode & S_IFIFO) printf("p");
		else if (mode & S_IFREG) printf("-");
		else printf("?");
		if (mode & S_IRUSR) printf("r"); else printf("-");
		if (mode & S_IWUSR) printf("w"); else printf("-");
		if (mode & S_IXUSR) printf("x"); else printf("-");
		if (mode & S_IRGRP) printf("r"); else printf("-");
		if (mode & S_IWGRP) printf("w"); else printf("-");
		if (mode & S_IXGRP) printf("x"); else printf("-");
		if (mode & S_IROTH) printf("r"); else printf("-");
		if (mode & S_IWOTH) printf("w"); else printf("-");
		if (mode & S_IXOTH) printf("x"); else printf("-");
		printf (" ");
		struct passwd* user;
		printf("%d ", (int)(buf.st_nlink));
		user = getpwuid(buf.st_uid);
		printf("%s ", user->pw_name);
			
		printf("%ld %ld ", buf.st_rdev, buf.st_size);
		printf("%s\n", file->d_name);
		}
	}
	closedir(dir);
	return 0;
}


int main(int argc, char** argv) {
	if (argc == 2 && !strcmp(argv[1], "ls")) {
		ls("./");
	}
	else{
		if(argc == 3 && !strcmp(argv[1], "ls") && !strcmp(argv[2], "-l")){
		ls_l("./");
		}
	}



	_exit (0);
}