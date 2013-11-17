#ifdef PROCESSOR_H
#define PROCESSOR_H

#include "header.h"

class MasterProcessor {
public:
	MasterProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int **proc_pip0, int **idle_pip0);
	~MasterProcessor() {
		for (vector::iterator it=all_processes.begin(); it != all_processes.end(); it++) {
			if (*it != NULL) delete *it;
		}
	}

protected: 
	MasterProcessor(){}

private:
	void shortTerm();
	void midTerm();
	void longTerm();
	pthread_t pt[3];
	ReadyMLFQ &rq;
	BlockQueue &bq;
	int **proc_pip;
	int **idle_pip;
	vector<Proc *> all_processes; //Store all the processes created from long-term scheduler
};


class SlaveProcessor {
public:
	SlaveProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int *proc_pip0, int *idle_pip0);

protected:
	SlaveProcessor(){}

private:
	void running();
	pthread_t pt;
	ReadyMLFQ &rq;
	BlockQueue &bq;
	int *s_proc_pip;
	int *s_idle_pip;
};

//This class wrap process and running time together, in order to pass this info to slave processor
class ProcAndTime() {
public:
	ProcAndTime(Proc *pro0, int timeQuan0) {
		pro = pro0;
		timeQuan = timeQuan0;
	}
	Proc *pro;
	int timeQuan;
protected:
	ProcAndTime(){};
}

#endif
