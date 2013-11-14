#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "blockqueue.h"

BlockQueue::BlockQueue(){
    pthread_mutex_init(&blockQueueMutex,NULL);
    blockQueue.clear();
}

int BlockQueue::checkIO(Proc *procPtr){
    synchronized(blockQueueMutex){
        std::list<Proc>::iterator iter;
        for(iter=blockQueue.begin();iter!=blockQueue.end();iter++){
            Proc temp=*iter;
            if(!temp.isBlocked()){
                blockQueue.erase(iter);
                *procPtr=temp;
                return 0;
            }
        }
        return NO_PROCESS_NONBLOCKED;
    }
}

void BlockQueue::putProc(Proc process){
    synchronized(blockQueueMutex){
        blockQueue.push_back(process);
    }
}

#endif
