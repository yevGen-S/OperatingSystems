#include "sys/sem.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#define BUFF_SIZE 10
#define N_CONSUMER_THREADS 3
#define N_PRODUCER_THREADS 3

struct {
    int buff[BUFF_SIZE];
    sem_t *nempty;
    sem_t *nstored;
} shared;

pthread_mutex_t mutex;

int count = 0;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv) {
    pthread_t tid_produce[N_PRODUCER_THREADS];
    pthread_t tid_consume[N_CONSUMER_THREADS];

    // создание трех семафоров
    pthread_mutex_init(&mutex, NULL);
    sem_init(&shared.nempty, BUFF_SIZE, BUFF_SIZE);
    sem_init(&shared.nstored, 0, BUFF_SIZE);;

    for ( int i = 0; i < N_PRODUCER_THREADS; i++) {
        if(!(pthread_create(&tid_produce[i], NULL, produce, NULL))) {
            perror("Error of creating thread ...");
        }
    }

    for ( int i = 0; i < N_CONSUMER_THREADS; i++) {
        if(!(pthread_create(&tid_consume[i], NULL, consume, NULL))) {
            perror("Error of creating thread ...");
        }
    }


    for ( int i = 0; i < N_PRODUCER_THREADS; i++ ) {
        pthread_join(tid_produce[i], NULL);
    }

    for ( int i = 0; i < N_CONSUMER_THREADS; i++ ) {
        pthread_join(tid_consume[i], NULL);
    }


//    sem_unlink(...);
//    sem_unlink(...);
//    sem_unlink(...);
    exit(0);
}


void *produce(void *arg) {
    for (;;) {
        if (count == BUFF_SIZE - 1) {
            count = 0;
        }

        sem_wait(&shared.nempty);
        pthread_mutex_lock(&mutex);

        shared.buff[count] = rand()%100;
        printf("Produced value : %d \n Thread id: %lu\n Buff index: %d \n\n", shared.buff[count],
               pthread_self(), count);
        count++;

        sleep(2);

        pthread_mutex_unlock(&mutex);
        sem_post(&shared.nstored);
    }
}


void *consume(void *arg) {
    for (;;) {
        sem_wait(&shared.nstored);
        pthread_mutex_lock(&mutex);

        if (count == BUFF_SIZE - 1) {
            count = 0;
        }
        printf("Consume value from buff : %d\nThread id: %lu\n Buff index: %d \n\n", shared.buff[count],
               pthread_self(), count);
        count--;
        sleep(3);


        // ожидаем появления объекта в буфере
        pthread_mutex_unlock(&mutex);
        sem_post(&shared.nempty);
    }
}