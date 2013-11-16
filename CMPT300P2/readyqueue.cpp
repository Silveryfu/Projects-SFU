#include "readyqueue.h"

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    for(int i=0;i<3;i++){
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
        for(int i=0;i<3;i++){
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
    int result;
    synchronized(readyMLFQMutex){
        result=(readMLFQ[0].size()+readMLFQ[1].size()+readMLFQ[2].size())==0;
    }
    return result;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
}

