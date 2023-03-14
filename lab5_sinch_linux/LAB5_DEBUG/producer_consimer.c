#include "sys/sem.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stddef.h>
#include <unistd.h>

#include "ring_buffer.h"

#define BUFF_SIZE 10
//#define N_CONSUMER_THREADS 3
//#define N_PRODUCER_THREADS 3

struct {
    int buff[BUFF_SIZE];
    sem_t *nempty;
    sem_t *nstored;
    sem_t *mutex;
} shared_mem;


int count = 0;

void *produce(void*);
void *consume(void*);

int main(int argc, char **argv) {
    pthread_t tid_produce;
    pthread_t tid_consume;


//    RingBuffer* buffer = newRingBuffer(BUFF_SIZE);
//    shared_mem.buff = buffer;

    shared_mem.nempty = (sem_t*)malloc(sizeof(sem_t));
    shared_mem.nstored = (sem_t*)malloc(sizeof(sem_t));
    shared_mem.mutex = (sem_t*)malloc(sizeof(sem_t));

    if (shared_mem.nempty == NULL ||  shared_mem.nstored == NULL) {
        return 1;
    }

    // создание семафоров и мьютекса.
    sem_init(shared_mem.mutex, 1, 1);

    sem_init(shared_mem.nempty, BUFF_SIZE, BUFF_SIZE);
    sem_init(shared_mem.nstored, BUFF_SIZE, 0);;

    if((pthread_create(&tid_produce, NULL, &produce, NULL))!=0) {
        perror("Error of creating thread ...");
    }

    if((pthread_create(&tid_consume, NULL, &consume, NULL))!=0)
        perror("Error of creating thread ...");


    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);


//    deleteRingBuffer(shared_mem.buff);
    sem_unlink((const char *) shared_mem.nempty);
    sem_unlink((const char *) shared_mem.nstored);

    exit(0);
}


void *produce(void* arg) {

    for (int i = 0; i < BUFF_SIZE; i++) {

        sem_wait(shared_mem.nempty);
        sem_wait(shared_mem.mutex); // критическая секция

        int rand_val = rand()%1000;

        shared_mem.buff[i] = rand_val;
        printf("Produced value : %d \n Thread id: %lu\n Buff index: %d \n\n", rand_val,
               pthread_self(), i);

        sleep(1+rand()%2);
        sem_post(shared_mem.mutex);
        sem_post(shared_mem.nstored);
    }


    return NULL;
}


void *consume(void* arg) {

    for (int i = 0; i < BUFF_SIZE; i++) {

        sem_wait(shared_mem.nstored);
        sem_wait(shared_mem.mutex); // критическая секция


//        int *readVal;
//        RingBuffer_tryread(shared_mem.buff, readVal);
        printf("Consume value from buff : %d\nThread id: %lu\n Buff index: %d \n\n", shared_mem.buff[i],
               pthread_self(), i);
        sleep(1+rand()%2);


        // ожидаем появления объекта в буфере
        sem_post(shared_mem.mutex);
        sem_post(shared_mem.nempty);
    }


    return NULL;
}

