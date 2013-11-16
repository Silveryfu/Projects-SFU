#ifndef BlOCKQUEUE_H
#define BlOCKQUEUE_H
#include "monitor.h"
#include "proc.h"

class BlockQueue{
private:
    pthread_mutex_t blockQueueMutex;
    std::list<Proc *> blockQueue;
public:
    BlockQueue();

    Proc * checkIO();

    void putProc(Proc *process);
};
#endif