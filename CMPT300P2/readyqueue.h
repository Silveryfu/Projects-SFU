#ifndef READYQUEUE_H
#define READYQUEUE_H
#include "header.h"
#include "proc.h"

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    pthread_cond_t condc;
    static int const BOOST_TRIGGER = 30;
    std::queue<Proc *> *readyMLFQ[LEVEL];
    void priorityBoost();
    int boostCounter;
public:
    ReadyMLFQ();

    void putProc(Proc *process);

    Proc * getProc();

    int totalSize();

    ~ReadyMLFQ();
};
#endif