#include "readyqueue.h"

pthread_cond_t condc,condp;

ReadyMLFQ::ReadyMLFQ(){
    pthread_mutex_init(&readyMLFQMutex,NULL);
    pthread_cond_init(&condc, NULL);
    for(int i=0;i<LEVEL;i++){
        while(!readMLFQ[i].empty()){
        readMLFQ[i].pop();
        }
    }
}

void ReadyMLFQ::putProc(Proc *process){
    synchronized(readyMLFQMutex){
        readMLFQ[process->getPriority()-1].push(process);
		if(totalSize()==1) pthread_cond_signal(&condc);
	}
}

Proc * ReadyMLFQ::getProc(){
	Proc *procPtr=NULL;
    synchronized(readyMLFQMutex){
		for(int i=0;i<LEVEL;i++){
            while(totalSize()==0) pthread_cond_wait(&condc, &readyMLFQMutex);
			if(!readMLFQ[i].empty()){
                procPtr=readMLFQ[i].front();
                readMLFQ[i].pop();
                break;
            }
        }
    }
    return procPtr;
}


int ReadyMLFQ::totalSize(){
    int sum=0;
    for(int i=0;i<LEVEL;i++){
        sum+=readMLFQ[i].size();
    }
    return sum;
}

ReadyMLFQ::~ReadyMLFQ(){
    pthread_mutex_destroy(&readyMLFQMutex);
    pthread_cond_destroy(&condc);	
}

