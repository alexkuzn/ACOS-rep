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


int main (int argc, char* argv[]) {
	int pipe_fd[2];
	if (pipe (pipe_fd) == -1) {
		perror ("can't make pipe");
		close (pipe_fd[0]);
		close (pipe_fd[1]);
		exit(1);
	}

	int pid = fork();
	if (pid == -1) {
		perror ("unable to fork");
	}
	else
		if (!pid) {		//child
			close (pipe_fd[0]);
			dup2 (pipe_fd[1], 1);
			if (execvp (argv[1], argv + 1) == -1) {
				printf("can't use exec vp for %s", argv[1]);
				perror ("can't use exec vp");
			}
			close (pipe_fd[1]);
			exit (0);
		}
		else {		//parent
			close (pipe_fd[1]);
			char *buf = malloc (12000);
			int t = 0;
			int line_quantity = 0;
			while (t = read (pipe_fd[0], buf, 12000)) {
				int i = 0;
				while (i < t) {
					printf("%c", buf[i]);
					if (buf[i] == '\n') {
						line_quantity++;
					}
					i++;
				}
			}
			printf("THE AMOUNT OF LINE IS %d\n", line_quantity);
		}
	exit(0);
}















