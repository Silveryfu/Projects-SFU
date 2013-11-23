#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "proc.h"

class Proc;

class BlockQueue{
private:
    pthread_mutex_t blockQueueMutex;
    std::list<Proc *> blockQueue;
public:
    BlockQueue();

    Proc * checkIO();

    std::vector<Proc *> getList();

    void putProc(Proc *process);
};
#endif
