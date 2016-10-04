#include <stdio.h>
#include <pthread.h>

void * myThread (void * parameter) {
    puts ("Hello Thread!\n");
    return NULL;
    }

int main() {
    pthread_t th;
    pthread_create(&th, NULL, myThread, NULL);

    pthread_join(th,NULL);
    puts("Hello Main!\n");
    return 0;
    }
