#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "monitor.h"
#include "proc.h"
#include <queue>

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    queue<Proc *> readMLFQ[3];
public:
    ReadyMLFQ();

    void putProc(Proc *process);

    Proc * getProc();

    int isEmpty();

    ~ReadyMLFQ();
};
#endif