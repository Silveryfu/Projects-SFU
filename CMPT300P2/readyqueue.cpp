#include "readyqueue.h"

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    pthread_cond_init(&condc, NULL);
    boostCounter=0;
    for(int i=0;i<LEVEL;i++){
        readyMLFQ[i]=new std::queue<Proc *>;
    }
}

void ReadyMLFQ::putProc(Proc *process){
    synchronized(readyMLFQMutex){
        readyMLFQ[process->getPriority()-1]->push(process);
		if(totalSize()==1) pthread_cond_signal(&condc);
	}
}

Proc * ReadyMLFQ::getProc(){
	Proc *procPtr=NULL;
    synchronized(readyMLFQMutex){
		for(int i=LEVEL-1;i>=0;i--) {
            while(totalSize()==0) pthread_cond_wait(&condc, &readyMLFQMutex);
			if(!readyMLFQ[i]->empty()){
                procPtr=readyMLFQ[i]->front();
                readyMLFQ[i]->pop();
                boostCounter++;
                if(boostCounter>=BOOST_TRIGGER){
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
        sum+=readyMLFQ[i]->size();
    }
    return sum;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
    pthread_cond_destroy(&condc);
    for(int i=0;i<LEVEL;i++){
        delete(readyMLFQ[i]);
    }
}

void ReadyMLFQ::priorityBoost(){
    for (int i=LEVEL-2; i>=0; i--) {
        while(!readyMLFQ[i]->empty()) {
            readyMLFQ[LEVEL-1]->push(readyMLFQ[i]->front());
            readyMLFQ[i]->pop();
        }
    }
}

