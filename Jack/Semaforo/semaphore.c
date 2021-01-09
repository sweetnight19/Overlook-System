// Exemple de threads sense protegir amb semafors 

// Includes propios
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"

static int glob = 0;
semaphore sem;

static void errExitEN (int s, const char *msg){
	printf ("%s: %s\n", msg, strerror (s));
	exit (EXIT_FAILURE);
}

static void * threadFunc (void *arg)
{
	int loops = *((int *) arg);
	int j;

	for (j = 0; j < loops; j++){
		SEM_wait(&sem);	
		glob++;
		SEM_signal(&sem);	
	}
	return NULL;
}

int main (int argc, char *argv[]){
	pthread_t t1, t2;
	int loops, s;

	SEM_constructor(&sem);
	SEM_init(&sem,1);

	loops = (argc > 1) ? atoi (argv[1]) : 10000000;

	s = pthread_create (&t1, NULL, threadFunc, &loops);
	
	if (s != 0)
	{
		errExitEN (s, "pthread_create");
	}

	s = pthread_create (&t2, NULL, threadFunc, &loops);
	
	if (s != 0)
	{
		errExitEN (s, "pthread_create");
	}
	
	s = pthread_join (t1, NULL);

	if (s != 0)
	{
		errExitEN (s, "pthread_join");
	}	
	
	s = pthread_join (t2, NULL);
	
	if (s != 0)
	{
		errExitEN (s, "pthread_join");
	}

	printf ("glob = %d\n", glob);

	SEM_destructor(&sem);

	exit (EXIT_SUCCESS);
}
