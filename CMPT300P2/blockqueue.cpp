#include "blockqueue.h"

BlockQueue::BlockQueue(){
    pthread_mutex_init(&blockQueueMutex,NULL);
    blockQueue.clear();
}

Proc * BlockQueue::checkIO(){
    Proc *procPtr=NULL;
    synchronized(blockQueueMutex){
        list<Proc *>::iterator iter;
        for(iter=blockQueue.begin();iter!=blockQueue.end();iter++){
            if(!(*iter)->isBlocked()){
                blockQueue.erase(iter);
                procPtr=(*iter);
                break;
            }
        }
    }
    return procPtr;
}

std::vector<Proc *> BlockQueue::getList(){
    std::vector<Proc *> temp;
    list<Proc *>::iterator iter;
    for(iter=blockQueue.begin();iter!=blockQueue.end();iter++){
        temp.push_back(*iter);
    }
    return temp;
}

void BlockQueue::putProc(Proc *process){
    synchronized(blockQueueMutex){
        blockQueue.push_back(process);
    }
}
