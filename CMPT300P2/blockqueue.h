#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "monitor.h"
#include "proc.h"
#include <list>

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