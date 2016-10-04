#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "protected_buffer.h"

#define MAXDURATION 5
#define MAXTHREADS 256
int last_thread = -1;
int first_thread = 0;
pthread_t threads_table[MAXTHREADS];
protected_buffer_t * protected_buffer;

// Terminate thread
void process_exit(int * seconds) {
		pthread_exit(seconds);
}

// Find a terminated child process and return its tid
pthread_t process_wait() {
		pthread_t thread = NULL;

		if (first_thread <= last_thread) {
				thread = threads_table[first_thread];
				first_thread++;
		}

		return thread;
}

// Main procedure for a child process
void * thread_main(void * arg){
		int * seconds = (int *) arg;

		sleep(*seconds);
		protected_buffer_put(protected_buffer, (void *) pthread_self());
		process_exit(seconds);
		return NULL;
}

int main(int argc, char *argv[]){
		struct timeval t;
		struct timeval s = {0, 0};

		if (argc != 2){ 
				printf("Usage : %s <n_threads>\n", argv[0]);
				exit(1);
		} 

		srandom(time(NULL));
		gettimeofday(&s, NULL);
		last_thread = atoi(argv[1]) - 1;
		protected_buffer = protected_buffer_init(last_thread);

		// Creer autant de threads que demandé en ligne de commande
		int n;
		int * seconds;
		for (n = 0; n <= last_thread; n++) {
				seconds = (int *)malloc(sizeof(int));
				*seconds = random()%MAXDURATION + MAXDURATION/2;
				pthread_create(&threads_table[n], NULL, thread_main, seconds);
				printf("thread (%p) sleeps for %d s\n", (void *)threads_table[n], *seconds);
		}

		// Attendre la terminaison des threads dans un certain ordre
		pthread_t thread;
		for (n = 0; n <= last_thread; ++n) {
				thread = (pthread_t) protected_buffer_get(protected_buffer);
				gettimeofday(&t, NULL);
				printf("thread (%p) join after %d s\n", (void *)thread, (int)(t.tv_sec -s.tv_sec));
		}
		return 0;

}
