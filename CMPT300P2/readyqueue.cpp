#include "readyqueue.h"

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    for(int i=0;i<3;i++){
        while(!readMLFQ[i].empty()){
        readMLFQ[i].pop();
        }
    }
}

void ReadyMLFQ::putProc(Proc process){
    synchronized(readyMLFQMutex){
    //pthread_mutex_lock(&readyMLFQMutex);{
        readMLFQ[process.getPriority()-1].push(process);
    }
    //pthread_mutex_unlock(&readyMLFQMutex);
}

int ReadyMLFQ::getProc(Proc *procPtr){
    int i;
    synchronized(readyMLFQMutex){
    //pthread_mutex_lock(&readyMLFQMutex);{
        for(i=0;i<3;i++){
            if(!readMLFQ[i].empty()){
                *procPtr=readMLFQ[i].front();
                readMLFQ[i].pop();
                break;
            }
        }
    }
    //pthread_mutex_unlock(&readyMLFQMutex);
    if(i==3)
        return NO_PROCESS_IN_QUEUE;
    return 0;
}

int ReadyMLFQ::size(){
    return readMLFQ[0].size()+readMLFQ[1].size()+readMLFQ[2].size();
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
}

