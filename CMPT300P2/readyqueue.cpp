#include "readyqueue.h"

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    for(int i=0;i<LEVEL;i++){
        while(!readMLFQ[i].empty()){
        readMLFQ[i].pop();
        }
    }
}

void ReadyMLFQ::putProc(Proc *process){
    synchronized(readyMLFQMutex){
        readMLFQ[process.getPriority()-1].push(process);
    }
}

Proc * ReadyMLFQ::getProc(){
    Proc *procPtr=NULL;
    synchronized(readyMLFQMutex){
        for(int i=0;i<LEVEL;i++){
            if(!readMLFQ[i].empty()){
                procPtr=readMLFQ[i].front();
                readMLFQ[i].pop();
                break;
            }
        }
    }
    return procPtr;
}

int ReadyMLFQ::isEmpty(){
    int sum=0;
    synchronized(readyMLFQMutex){
        for(int i=0;i<LEVEL;i++){
            sum+=readMLFQ[i].size();
        }
    }
    return sum==0;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
}

