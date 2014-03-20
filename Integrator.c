#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

double result = 0;

double function (double x) {
	printf ("f(x) = %lf \n", x);
	return x * x;
}


double integrator (double a, double b) {
	return 0.5 * (b - a) * (function(b) + function(a));
}



int main (int argc, char* argv[]) {

	double temporary_result;
	FILE *fp;
	fp = fopen ("result.txt", "w+");
	fprintf(fp, "%f", result);
	fclose (fp);
	//fseek (fp, 0, SEEK_SET);
	int rang = atoi(argv[1]);			//vvodim chislo kratnoe 3
	int n = rang;
	rang /= 3;
	double range [n];
	int k = rang;
	for (int i = 1; i < n; i ++) {
		if (i % 2 || i == 1) {
			range[i] = k;
			//rang += rang;
		}
		else {
			range[i] = k;
			k += rang;
		}
		printf("%lf \n", range[i]);
	}


	for (int i = 1; i < n; i += 2) {

		int pid = fork();
		if (pid == -1) {	
			perror ("unable to fork");
			exit (1);
		}
		else
			if (!pid) {		//child

		

				fp = fopen ("result.txt", "r+");


				printf("IM A CHILD NUMBER %d \n", i);
				fscanf (fp, "%lf", &result);
				fseek (fp, 0, SEEK_SET);
				//result += 5;
				printf("I got a number = %lf \n", integrator(range[i - 1], range[i]));
				result += integrator (range[i - 1], range[i]);


				fprintf(fp, "%lf", result);
				//fseek (fp, 0, SEEK_SET);
				fclose (fp);
				exit(0);
			}
			else {
				waitpid (pid, 0, 0);
			}
	}
	fp = fopen ("result.txt", "r+");

	fscanf (fp, "%lf", &result);
	printf ("the result is %lf", result);
	fclose (fp);

	exit (0);
}





