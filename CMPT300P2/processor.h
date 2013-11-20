#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "header.h"
#include "readyqueue.h"
#include "blockqueue.h"

class ReadyMLFQ;
class BlockQueue;

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


class SlaveProcessor {
public:
	SlaveProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int *s_proc_pip0, int *s_idle_pip0, int slaveID0);
	static void *run(void *self){
        ((SlaveProcessor*)self)->running();
        return NULL;
    }
protected:
	SlaveProcessor(){}

private:
	void running();
	pthread_t pt;
	ReadyMLFQ *rq;
	BlockQueue *bq;
	char indent[SLAVE_INDENT_WIDTH*SLAVES_NUMBER];
	int *s_proc_pip;
	int *s_idle_pip;
	int slaveID;
};

//This class wrap process and running time together, in order to pass those info to slave processor
class ProcWrapper{
public:
	ProcWrapper(Proc *pro0, int timeQuanta0) {
		pro = pro0;
		timeQuanta = timeQuanta0;
	}
	Proc *pro;
	int timeQuanta;
protected:
	ProcWrapper(){};
};

#endif
