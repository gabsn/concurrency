#include "readers_writers_mutexes.h"

void rw_mutex_init (rw_mutex_t * rw_mutex){
		rw_mutex->n_readers = 0;
		rw_mutex->n_writers = 0;
		pthread_mutex_init(&(rw_mutex->mutex), NULL);
		pthread_mutex_init(&(rw_mutex->r_mutex), NULL);
		pthread_mutex_init(&(rw_mutex->w_mutex), NULL);
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : enter r = %d v = %ld\n", conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_read_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : leave r = %d v = %ld\n", conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
		pthread_mutex_lock(&(rw_mutex->w_mutex));
		rw_mutex->n_writers++;
		pthread_mutex_unlock(&(rw_mutex->w_mutex));
		while (rw_mutex->n_readers>0);
		pthread_mutex_lock(&(rw_mutex->mutex));
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
		pthread_mutex_unlock(&(rw_mutex->mutex));
		pthread_mutex_lock(&(rw_mutex->w_mutex));
		rw_mutex->n_writers--;
		pthread_mutex_unlock(&(rw_mutex->w_mutex));
}
