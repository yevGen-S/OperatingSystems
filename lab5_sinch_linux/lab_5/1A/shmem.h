//
// Created by evgenii on 01.11.22.
//

#ifndef LAB_5_SHMEM_H
#define LAB_5_SHMEM_H

#define MAXBUFF 80 //максимальная длина сообщения вразделяемой памяти
#define PERM 0666 //права доступа к разделяемой памяти
typedef struct mem { //структура данных в разделяемой памяти
    int segment;
    char buff[MAXBUFF];
}Message;

//ожидание начала выполнения клиента
static struct sembuf proc_wait[1] = {1, -1, 0};

//уведомление сервера о том, что клиент начал работу
static struct sembuf proc_start[1] = {1, 1, 0};

//блокирование разделяемой памяти
static struct sembuf mem_lock[2] = {0, 0, 0, 0, 1, 0};

//освобождение ресурса
static struct sembuf mem_unlock[1] = {0, -1, 0};

#endif //LAB_5_SHMEM_H
