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
    printf("PutProc\n");
    synchronized(readyMLFQMutex){
        boostCounter++;
        if(boostCounter>=BOOST_TRIGGER){
            priorityBoost();
            boostCounter=0;
        }
        readyMLFQ[process->getPriority()-1]->push(process);
        //If size of the ready queue is just becoming positive, then send a signal to wake up the waited thread.
		if(totalSize()==1) pthread_cond_signal(&condc);
	}
    printf("PutProc End\n");
}

Proc * ReadyMLFQ::getProc(){
	Proc *procPtr=NULL;
    printf("GetProc\n");
    synchronized(readyMLFQMutex){
		for(int i=LEVEL-1;i>=0;i--) {
            //Wait when the ready queue is empty. It will block and wait for signal.
            printf("Start\n");
            while(totalSize()==0) pthread_cond_wait(&condc, &readyMLFQMutex);
            printf("End\n");
			if(!readyMLFQ[i]->empty()){
                procPtr=readyMLFQ[i]->front();
                readyMLFQ[i]->pop();
                break;
            }
        }
    }
    printf("GetProc End\n");
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
            Proc *ptrProc=readyMLFQ[i]->front();
            ptrProc->changePriority(LEVEL-1-i);
            readyMLFQ[LEVEL-1]->push(ptrProc);
            readyMLFQ[i]->pop();
        }
    }
}

