#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "monitor.h"
#include "proc.h"
#include <queue>

int const NO_PROCESS_IN_QUEUE=1;

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    queue<Proc> readMLFQ[3];
public:
    ReadyMLFQ();

    void putProc(Proc process);

    int getProc(Proc *procPtr);

    int size();

    ~ReadyMLFQ();
};
#endif