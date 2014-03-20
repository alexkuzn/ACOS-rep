#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>


void file_to_file_cp (char* from, char* to1, bool b_is_dir) {	
	char* to;
	//to = malloc(strlen(to1) + strlen(name) + 6 * sizeof(char));
	if (b_is_dir) {
		char* name = strrchr(from, '/')	;
		if (!name)
			name = from;
		else name = name + 1;
		to = malloc(strlen(to1) + strlen(name) + 6 * sizeof(char));
		strcpy(to, to1);
		strcat(to, "/");
		strcat(to, name);
	}
	else {
		to = to1;
	}

	//printf("FILE TO FILE IS WORKING, TO IS :  %s\n", to);
	//printf("FILE TO FILE IS WORKING, FROM IS :  %s\n", from);

	int fd1 = open (from, O_RDONLY);
	if (fd1 < 0) {
		perror ("unable to open 1st file");
		close (fd1);
		_exit(1);
		
	}
	struct stat buf1;
	stat (from, &buf1);
	int fd2 = open (to, O_WRONLY | O_TRUNC | O_CREAT, buf1.st_mode);
	if (fd2 < 0) {
		perror ("unable to open 2nd file");
		close (fd1);
		close(fd2);
		_exit(1);
	}
	chmod (to, buf1.st_mode);
	void *buf = malloc (1000);
	if (!buf) {
		write (2, "unable to get memory buf", strlen ("unable to get memory buf"));
		free (buf);
		close (fd1);
		close(fd2);
		_exit(1);
	}
	int n, count = 1000;
	while ((n = read (fd1, buf, count)) > 0) {
		int k = write (fd2, buf, n);
		if (k < 0) {
			perror ("unable to write");
			free (buf);
			close (fd1);
			close(fd2);
			_exit(1);
		}

	}
	if (n < 0) {
			perror ("unable to read");
			free (buf);
			close (fd1);
			close(fd2);
			_exit(1);
	}
	free (buf);
	close (fd1);
	close(fd2);
}



void dir_dir_cp (char* from, char* to) {
	//printf ("%s -> %s \n", from, to);

	DIR* dir_from = opendir (from);	//error
	if (!dir_from) {
		peror ("unable to open dir");
		exit(1);
	}
	struct dirent *file;
	struct stat buf_from, buf_to;
	stat (from, &buf_from);		//error
	stat (from, &buf_to);

	mkdir (to, buf_to.st_mode);

	char* name = strrchr(from, '/');
	if (!name)
		name = from;
	else name = name + 1;
	char*  to_new = malloc(strlen(to) + strlen(name) + 2 * sizeof(char));
	strcpy(to_new, to);
	strcat(to_new, "/");
	strcat(to_new, name);

	mkdir (to_new, buf_from.st_mode);
	//printf("TO NEW =%s\n", to_new);


	while ((file = readdir(dir_from)) != 0) {
		if (strcmp (file->d_name, ".") != 0 && strcmp (file->d_name, "..") != 0) {
		if (file->d_type & DT_REG) {	//just a file in from directory
			char* from_new = malloc (strlen(from) + strlen (file->d_name) + 2 * sizeof(char));
			strcpy (from_new, from);
			strcat (from_new, "/");
			strcat (from_new, file->d_name);
			//printf("%s\n", from_new);
			file_to_file_cp (from_new, to_new, true);
			free (from_new);
		}
		else {
			if (file->d_type & DT_DIR) {	//the directory in form directory
				char* from_new = malloc (strlen(from) + strlen (file->d_name) + 2 * sizeof(char));
				strcpy (from_new, from);
				strcat (from_new, "/");
				strcat (from_new, file->d_name);
				dir_dir_cp (from_new, to_new);
				free (from_new);
			}
		}
		}
	}
	//printf ("KONEC \n");
	free (to_new);
}




void main_cp (int argc, char** argv) {
	int i = 1;
	bool is_i_incr = false;
	struct stat buf_stat;
	bool a_is_dir = false;
	bool b_is_dir = false;
	bool r = false;
	if (argc > 2) {
		while (i < argc - 1) {
			if (!strcmp (argv[1], "-R") && (i + 1 < argc - 1 || is_i_incr)) {
				//printf("-R IS HERE\n");
				if (is_i_incr == false){
					i++;
					is_i_incr = true;
				}
				stat (argv[i], &buf_stat);
				//printf ("ARGV[i] = %s\n", argv[i]);
				
				if (buf_stat.st_mode & S_IFDIR) {
					a_is_dir = true;
				}
				stat (argv[argc - 1], &buf_stat);
				if (buf_stat.st_mode & S_IFDIR) {
					b_is_dir = true;
				}
			}
			if (a_is_dir) {
				if (b_is_dir) {
					//printf ("HELLO IM WORKING \n");
					//printf ("ARGV[i] = %s\n", argv[i]);
					dir_dir_cp (argv[i], argv[argc - 1]);
				}
			}
			else {
				//printf ("ARGV[i] = %s\n", argv[i]);
				file_to_file_cp (argv[i], argv[argc - 1], b_is_dir);
			}
			i++;
			}
			
	}
}


int main (int argc, char** argv) {
	if (argc < 3) {
		write (2, "too less arguments", strlen("too less arguments"));
		_exit(1);
	}
	else
		main_cp (argc, argv);
	
	_exit(0);
}