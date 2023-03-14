
#include "ring_buffer.h"


RingBuffer* newRingBuffer(unsigned long int buff_size) {
    int* mem = malloc(buff_size * INT_SIZE);
    if(mem == NULL) {
        return NULL;
    }

    RingBuffer* rb = malloc(sizeof(RingBuffer));
        if(rb == NULL) {
            free(mem);
            return NULL;
        }

    rb->start = mem;
    rb->end = mem + buff_size;
    rb->readptr = mem;
    rb->writeptr = mem;

    return rb;
}

void deleteRingBuffer(RingBuffer* rb) {
    if(rb == NULL) return;

    free(rb->start);
    free(rb);
}

int RingBuffer_trywrite(RingBuffer* rb, int c) {
    if(rb == NULL)
        return RINGBUFFER_ERR_NULL;

    if(rb->writeptr + 1 == rb->readptr)
        return RINGBUFFER_ERR_FULL;


    *(rb->writeptr) = c;

    int* tmp = rb->writeptr + 1;
    if(tmp >= rb->end) tmp = rb->start;
    rb->writeptr = tmp;

    return RINGBUFFER_OK;
}

int RingBuffer_tryread(RingBuffer* rb, int* c) {
    if(rb == NULL)
        return RINGBUFFER_ERR_NULL;

    if(rb->readptr == rb->writeptr)
        return RINGBUFFER_ERR_EMPTY;


    c = (rb->readptr);

    int* tmp = rb->readptr + 1;

    if(tmp >= rb->end)
        tmp = rb->start;

    rb->readptr = tmp;

    return RINGBUFFER_OK;
}