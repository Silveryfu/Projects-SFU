#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "header.h"

class BlockQueue{
private:
    pthread_mutex_t blockQueueMutex;
    list<Proc *> blockQueue;
public:
    BlockQueue();

    Proc * checkIO();

    void putProc(Proc *process);
};
#endif