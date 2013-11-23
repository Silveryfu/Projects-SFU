#include "blockqueue.h"

BlockQueue::BlockQueue(){
    pthread_mutex_init(&blockQueueMutex,NULL);
}

Proc* BlockQueue::getProc(){
    Proc* tmp=NULL;
    synchronized(blockQueueMutex){
	if(!blockQueue.empty()){
	tmp=blockQueue.top();
    	blockQueue.pop();
	}
    }
    return tmp;
}

void BlockQueue::putProc(Proc *process){
    
     synchronized(blockQueueMutex){
	blockQueue.push(process);
     }
}
