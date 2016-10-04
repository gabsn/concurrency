#include <pthread.h>
#include <stdlib.h>
#include "circular_buffer.h"
#include "protected_buffer.h"

protected_buffer_t * protected_buffer_init(int length) {
		protected_buffer_t * b;
		b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
		b->buffer = circular_buffer_init(length);
		// Initialize the synchronization components
		return b;
}

void * protected_buffer_get(protected_buffer_t * b){
		void * d;
		// Enter mutual exclusion
		pthread_mutex_lock(&(b->m));
		// Wait until there is a full slot to get data from the unprotected
		// circular buffer (circular_buffer_get).
		while (circular_buffer_size(b->buffer) == 0)
				pthread_cond_wait(&(b->v), &(b->m));
		d = circular_buffer_get(b->buffer);
		// Signal or broadcast that an empty slot is available in the
		// unprotected circular buffer.
		pthread_cond_broadcast(&(b->v));
		// Leave mutual exclusion
		pthread_mutex_unlock(&(b->m));
		return d;
}

int protected_buffer_put(protected_buffer_t * b, void * d){
		// Enter mutual exclusion
		pthread_mutex_lock(&(b->m));
		// Wait until there is an empty slot to put data in the unprotected
		// circular buffer (circular_buffer_put).
		while (circular_buffer_size(b->buffer) == circular_buffer_maxsize(b->buffer))
				pthread_cond_wait(&(b->v), &(b->m));
		circular_buffer_put(b->buffer, d);
		// Signal or broadcast that a full slot is available in the
		// unprotected circular buffer.
		pthread_cond_broadcast(&(b->v));
		// Leave mutual exclusion
		pthread_mutex_unlock(&(b->m));
		return 1;
}
