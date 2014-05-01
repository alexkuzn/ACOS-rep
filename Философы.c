#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t forks[100];
pthread_t philosophers[100];
int n;

void* process (void *k)
{
	int id = (int) k;
	int dur;
	srandom (time(NULL));
	dur = (rand() % 5) + 1;
	//sleep (dur);
	int id_fork = id;
	if (id % 2 == 0)
	{
		id_fork --;
		if (id_fork == -1)
		{
			id_fork = n - 1;
		}
	}

	printf("Философ номер %d спал %d\n", id, dur);
	printf("Пытается взять вилку номер : %d\n", id_fork);

	if (pthread_mutex_lock (&(forks[(id_fork)])))
	{
		printf("ERROR MUTEX LOCK %d\n", id_fork);
	}

	printf("Взял 1ую вилку номер: %d, филосов номер %d\n", id_fork % n, id);

	if (id % 2 == 1)
	{
		id_fork --;
	}
	else
	{
		id_fork ++;
	}

	while (pthread_mutex_trylock(&forks[(id_fork % n)]))
	{
		/*if (id % 2 == 1)
		{
			id_fork ++;
		}
		else
		{
			id_fork --;
			if (id_fork == -1)
			{
				id_fork = n - 1;
			}
		}
	}

		pthread_mutex_unlock (&forks[id_fork]);*/
		sleep (dur = (rand() % 10) + 5);
		
		printf("Философ номер %d ждал %d\n", id, dur);
}

	printf("Взял 2ую вилку %d филосов номер %d\n", (id_fork % n), id);
	//sleep (dur = ((rand() % 10) + 1));
	printf("Философ номер %d ел %d\n", id, dur);
	pthread_mutex_unlock (&forks[id_fork % n]);
	if (id % 2 == 1)
	{
		id_fork ++;
	}
	else
	{
		id_fork --;
		if (id_fork == -1)
		{
			id_fork = n - 1;
		}
	}
	pthread_mutex_unlock (&forks[(id_fork) % n]);
	pthread_exit (NULL);
}

int main (int argc, char* argv[])
{
	printf("%s\n", argv[1]);
	n = atoi (argv[1]);
	int i = 0;
	for (i = 0; i < n; i++)
	{
		if (pthread_mutex_init (&forks[i], NULL))
			printf("ERROR MUTEX INIT");
	}
	for (i = 0; i < n; i++)
	{
		if (pthread_create (&philosophers[i], NULL, process, (void*) i))
			printf("ERROR PTHREAD CREATE");
	}
	void* result;
	for (i = 0; i < n; i++) {
		pthread_join(philosophers[i], &result);
	}
	for (i = 0; i < n; i++)
	{

		pthread_mutex_destroy (&forks[i]);
	}
}


