#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "header.h"

int const LEVEL=3;

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    queue<Proc *> readMLFQ[LEVEL];
public:
    ReadyMLFQ();

    void putProc(Proc *process);

    Proc * getProc();

    int isEmpty();

    ~ReadyMLFQ();
};
#endif