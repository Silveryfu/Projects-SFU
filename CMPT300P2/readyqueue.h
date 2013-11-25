#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "proc.h"

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    pthread_cond_t condc;
    std::queue<Proc *> *readyMLFQ[LEVEL];
    void priorityBoost();
    int boostCounter;
    int totalSize();
public:
    ReadyMLFQ();

    void putProc(Proc *process);

    Proc * getProc();

    ~ReadyMLFQ();
};
#endif