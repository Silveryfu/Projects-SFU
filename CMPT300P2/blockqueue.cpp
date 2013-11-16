#include "blockqueue.h"

BlockQueue::BlockQueue(){
    pthread_mutex_init(&blockQueueMutex,NULL);
    blockQueue.clear();
}

int BlockQueue::checkIO(Proc *procPtr){
    int result=NO_PROCESS_NONBLOCKED;
    synchronized(blockQueueMutex){
        std::list<Proc>::iterator iter;
        for(iter=blockQueue.begin();iter!=blockQueue.end();iter++){
            Proc temp=*iter;
            if(!temp.isBlocked()){
                blockQueue.erase(iter);
                *procPtr=temp;
                result=0;
                break;
            }
        }
    }
    return result;
}

void BlockQueue::putProc(Proc process){
    synchronized(blockQueueMutex){
        blockQueue.push_back(process);
    }
}

int BlockQueue::getProc(Proc *procPtr){
    int i;
    synchronized(blockQueueMutex){
        if(!blockQueue.empty()){
            *procPtr=readMLFQ[i].front();
            readMLFQ[i].pop();
            break;
        }
    }
    if(i==3)
        return NO_PROCESS_IN_QUEUE;
    return 0;
}
