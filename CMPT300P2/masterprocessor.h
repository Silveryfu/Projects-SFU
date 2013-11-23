#ifndef MASTERPROCESSOR_H
#define MASTERPROCESSOR_H

#include "readyqueue.h"
#include "blockqueue.h"

class MasterProcessor {
public:
	MasterProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int proc_pip0[][2], int idle_pip0[][2]);
	~MasterProcessor() {
		for (std::list<Proc *>::iterator it=all_processes.begin(); it != all_processes.end(); it++) {
			delete *it;
		}
	}
	static void *runShortTermScheduler(void *self){
        ((MasterProcessor*)self)->shortTermScheduler();
        return NULL;
    }
    static void *runMidTermScheduler(void *self){
    	((MasterProcessor*)self)->midTermScheduler();
        return NULL;
    }
    static void *runLongTermScheduler(void *self){
    	((MasterProcessor*)self)->longTermScheduler();
    	return NULL;
    }
protected:
	MasterProcessor(){}


private:
	void shortTermScheduler();
	void midTermScheduler();
	void longTermScheduler();
	pthread_t pt[3];
	ReadyMLFQ *rq;
	BlockQueue *bq;
	int (*proc_pip)[2];
	int (*idle_pip)[2];
	std::queue<int> IDSpace;
	std::list<Proc *> all_processes; //Store all the processes created from long-term scheduler
};

#endif
