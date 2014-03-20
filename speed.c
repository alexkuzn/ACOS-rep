#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <pwd.h>

struct timeval time_begin, time_end, time_result;
struct timezone time_zone;


void start () {
	gettimeofday (& time_begin, & time_zone);
}
void end1 () {
	gettimeofday (& time_end, & time_zone);
}

long get_result_of_time () {
	time_result.tv_sec = - time_begin.tv_sec + time_end.tv_sec;
	time_result.tv_usec = - time_begin.tv_usec + time_end.tv_usec;
	if (time_result.tv_usec < 0) {	//
		time_result.tv_sec--;
		time_result.tv_usec += 1000000;
	}
	return time_result.tv_sec * 1000 + time_result.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
	int size = 5000000;			//x
	int pipe_fd[2];
	if (pipe (pipe_fd) == -1) {
		perror ("can't make pipe");
		close (pipe_fd[0]);
		close (pipe_fd[1]);
		exit(1);
	}
	//int i = 1;
	//for (; i < 4096; i++) {
		
		printf("HI! IM WORKING \n");

		int pid = fork();
		if (pid == -1) {	
			perror ("unable to fork");
			exit (1);
		}
		else
			if (!pid) {		//child
				printf("HI! IM WORKING \n");
				char *buf = malloc (size);
				int i = 0;
				while (i < size) {
					buf[i] = '1';
					i++;
				}
				char *buf1 = buf;
				close (pipe_fd[0]);

				long long size_to_write = 1000000000;
				long long cur_size = 0;

				start();

				while (cur_size < size_to_write) {
					long long real_size = (size > strlen(buf1)) ? strlen(buf1) : size;
					long long t = 0;
					while (cur_size < size_to_write && (t = write (pipe_fd[1], buf, real_size)) != 0) {
						buf1 += t;
						real_size -= t;
						cur_size += t;
					}
					cur_size += t;
					buf1 = buf;
				}
				end1();
				printf("HI! IM WORKINGGGGGGG \n");
				printf ("THE TIME IS:%ld \n", get_result_of_time());

				free (buf);
				close (pipe_fd[1]);
				exit (0);

			}
			else {			//parent
				void *buf = malloc (size);
				close (pipe_fd[1]);
				int t = 0;
				while ( (t = read (pipe_fd[0], buf, size)) != 0);
				close (pipe_fd[0]);
				free (buf);
			}

	//}



	exit(0);
}