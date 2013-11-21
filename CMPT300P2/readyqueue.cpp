#include "readyqueue.h"

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    pthread_cond_init(&condc, NULL);
    boostCounter=0;
    for(int i=0;i<LEVEL;i++){
        readyqueue[i]=new std::queue<Proc *>;
    }
}

void ReadyMLFQ::putProc(Proc *process){
    synchronized(readyMLFQMutex){
        readyMLFQ[process->getPriority()-1].push(process);
		if(totalSize()==1) pthread_cond_signal(&condc);
	}
}

Proc * ReadyMLFQ::getProc(){
	Proc *procPtr=NULL;
    synchronized(readyMLFQMutex){
		for(int i=LEVEL-1;i>=0;i--) {
            while(totalSize()==0) pthread_cond_wait(&condc, &readyMLFQMutex);
			if(!readyMLFQ[i].empty()){
                procPtr=readyMLFQ[i].front();
                readyMLFQ[i].pop();
                boostCounter++;
                if(boostCounter==BOOST_TRIGGER){
                    priorityBoost();
                    boostCounter=0;
                }
                break;
            }
        }
    }
    return procPtr;
}


int ReadyMLFQ::totalSize(){
    int sum=0;
    for(int i=0;i<LEVEL;i++){
        sum+=readyMLFQ[i].size();
    }
    return sum;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
    pthread_cond_destroy(&condc);
    for(int i=0;i<LEVEL;i++){
        delete(readyqueue[i]);
    }
}

void ReadyMLFQ::priorityBoost(){
    while(!readyMLFP[1].empty()){
        readyMLFP[0]->push(readyqueue[1]->front());
        readyMLFP[1].pop();
    }
    for(int index=1;index<LEVEL-1;index++){
        readyMLFP[index]=readyMLFP[index+1];
    }
    delete(readyMLFP[LEVEL-1]);
    readyMLFQ[LEVEL-1]=new std::queue<Proc *>;
}

