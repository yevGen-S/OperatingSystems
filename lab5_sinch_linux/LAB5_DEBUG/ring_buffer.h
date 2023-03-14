

#ifndef LAB5_DEBUG_RING_BUFFER_H
#define LAB5_DEBUG_RING_BUFFER_H

#include <stdlib.h>

#define INT_SIZE (sizeof(int))

#define RINGBUFFER_OK (0)
#define RINGBUFFER_ERR_NULL (-1)
#define RINGBUFFER_ERR_EMPTY (-2)
#define RINGBUFFER_ERR_FULL (-3)

typedef struct {
    int* start;
    int* end;
    volatile int* readptr;
    volatile int* writeptr;
} RingBuffer;

RingBuffer* newRingBuffer(unsigned long int buff_size);
void deleteRingBuffer(RingBuffer* rb);
int RingBuffer_trywrite(RingBuffer* rb, int c);
int RingBuffer_tryread(RingBuffer* rb, int* c);

#endif //LAB5_DEBUG_RING_BUFFER_H


