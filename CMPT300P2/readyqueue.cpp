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
        readMLFQ[process.getPriority()-1].push(process);
    }
}

int ReadyMLFQ::getProc(Proc *procPtr){
    int i;
    synchronized(readyMLFQMutex){
        for(i=0;i<3;i++){
            if(!readMLFQ[i].empty()){
                *procPtr=readMLFQ[i].front();
                readMLFQ[i].pop();
                break;
            }
        }
    }
    if(i==3)
        return NO_PROCESS_IN_QUEUE;
    return 0;
}

int ReadyMLFQ::isEmpty(){
    int result;
    synchronized(readyMLFQMutex){
        result=(readMLFQ[0].size()+readMLFQ[1].size()+readMLFQ[2].size())==0;
    }
    return result;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
}

