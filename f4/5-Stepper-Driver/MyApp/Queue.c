#include "Queue.h"

//Queue (Ring Buffer)
#define MOVE_BUFFER_SIZE 8

typedef struct
{
    MoveBlock buffer[MOVE_BUFFER_SIZE];
    volatile uint8_t head_MoveQueue;
    volatile uint8_t tail_MoveQueue;
} MoveQueue;
MoveQueue move_queue;


uint8_t Queue_Push(MoveBlock *block)
{
    uint8_t next = (move_queue.head_MoveQueue + 1) % MOVE_BUFFER_SIZE;

    if(next == move_queue.tail_MoveQueue)
        return 0; // queue full

    move_queue.buffer[move_queue.head_MoveQueue] = *block;
    move_queue.head_MoveQueue = next;

    return 1;
}


uint8_t Queue_Pop(MoveBlock *block)
{
    if(move_queue.head_MoveQueue == move_queue.tail_MoveQueue)
        return 0; // queue empty

    *block = move_queue.buffer[move_queue.tail_MoveQueue];
    move_queue.tail_MoveQueue = (move_queue.tail_MoveQueue + 1) % MOVE_BUFFER_SIZE;

    return 1;
}
