#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REINDEERS 9
static int waiting = 0;

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t reindeer_holiday_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeers_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t reindeers_flying_mutexes[REINDEERS] = {PTHREAD_MUTEX_INITIALIZER};

void* reindeer_func(void* arg) {
    int id = *(int*)arg;
    while (1) {
        printf("Reindeer %d is on holidays\n", id);
        sleep(5 + rand() % 6);

        pthread_mutex_lock(&reindeer_holiday_mutex);
        waiting++;
        printf("Reindeer %d: %d waiting\n", id, waiting);
        if (waiting == REINDEERS) {
            printf("Reindeer %d: waking up Santa\n", id);
            pthread_cond_signal(&santa_cond);   
        }
        pthread_mutex_unlock(&reindeer_holiday_mutex);
        pthread_mutex_lock(&reindeers_flying_mutexes[id]);
        while (waiting > 0) {
            pthread_cond_wait(&reindeers_cond, &reindeers_flying_mutexes[id]);    
        }
        pthread_mutex_unlock(&reindeers_flying_mutexes[id]);
    }
}

void* santa_func() {
    while (1) {
        printf("Santa is sleeping\n");
        pthread_cond_wait(&santa_cond, &santa_mutex);
        
        printf("Santa is riding\n");
        sleep(5);
        printf("Santa has returned\n");
        waiting = 0;
        pthread_cond_broadcast(&reindeers_cond);
    }
}

int main() {
    pthread_t santa;
    pthread_create(&santa, NULL, santa_func, NULL);
    int ids[REINDEERS];
    for (int i=0; i<REINDEERS; i++) {
        pthread_t tid;
        ids[i]= i;
        pthread_create(&tid, NULL, reindeer_func, &ids[i]);
    }
    pthread_join(santa, NULL);
    return 0;
}