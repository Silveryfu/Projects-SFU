#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "header.h"
#include "proc.h"

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    std::queue<Proc *> readMLFQ[LEVEL];
public:
    ReadyMLFQ();

    void putProc(Proc *process);

    Proc * getProc();

    int totalSize();

    ~ReadyMLFQ();
};
#endif