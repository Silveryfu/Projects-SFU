#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "proc.h"


class mycomparison
{
public:
  bool operator() (Proc*& lhs, Proc*& rhs)
  {
    return (lhs->getBlocTime() < rhs->getBlocTime());
  }
};

class BlockQueue{
private:
    pthread_mutex_t blockQueueMutex;
    std::priority_queue<Proc *, std::vector<Proc *>, mycomparison> blockQueue;
public:
    BlockQueue();
    Proc* getProc();

    void putProc(Proc *process);
};




#endif
