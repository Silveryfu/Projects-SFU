#ifndef SLAVEPROCESSOR_H
#define SLAVEPROCESSOR_H

#include "readyqueue.h"
#include "blockqueue.h"

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

#endif