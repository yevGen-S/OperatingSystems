#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

#include "shmem.h"

int main(void) {
    Message* msgptr;
    key_t key;
    int shmid, semid;


    //получение ключа
    if ((key = ftok("/home/evgenii/hardrive/home/yevs/Work_Study/Polytech/FIIT/OS/lab-7-virtual-mem-linux/key.txt", 'A')) == -1) {
        printf("Client: can't get a key\n");
        exit(-1);
    }


    //получение доступа к разделяемой памяти
    if ((shmid = shmget(key, sizeof(Message), 0)) == -1) {
        printf("Client: access denied\n");
        exit(-1);
    }


    //присоединение разделяемой памяти
    if ((msgptr = (Message*)shmat(shmid, 0, 0)) == NULL  ) {
        printf("Client : error of joining\n");
        exit(-1);
    }


    //получение доступа к семафору
    if ((semid = semget(key, 2, PERM)) == -1) {
        printf("Client: access denied\n");
        exit(-1);
    }

    //блокировка разделяемой памяти
    if (semop(semid, &mem_lock[0], 2) == -1) {
        printf("Client : can't execute a operation\n");
        exit(-1);
    }

    //уведомление сервера о начале работы
    if (semop(semid, &proc_start[0], 1) == -1) {
        printf("Client : can't execute a operation\n");
        exit(-1);
    }





    //запись сообщения в разделяемую память
    printf("Write full name of file to read content: ");
    fgets(msgptr->buff, MAXBUFF,  stdin); // for static size buff

    size_t len = 0;
//    msgptr->size = getline(&msgptr->buff, &len, stdin);

    printf("%s ", msgptr->buff);






    //освобождение ресурса
    if (semop(semid, &mem_unlock[0], 1) == -1) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }


    //ожидание завершения работы сервера с разделяемой памятью
    if (semop(semid, &mem_lock[0], 2) == -1) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }


    //чтение сообщения из разделяемой памяти
    printf("Client: Read response from server\n%s", msgptr->buff);


    //освобождение разделяемой памяти
    if (semop(semid, &mem_unlock[0], 1) == -1) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }

    //отключение от области разделяемой памяти
    if (shmdt(msgptr) == -1) {
        printf("Client: error\n");
        exit(-1);
    }
}