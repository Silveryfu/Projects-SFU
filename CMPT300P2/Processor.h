#include "header.h"

class MasterProcessor {
public:
	MasterProcessor(ReadyMLFQ &rq0, BlockQueue &bq0) {
		rq = rq0;
		bq = bq0;
    	if(pthread_create(&pt, NULL, &shortTerm, NULL)) {
    	    printf("Could not create shortTerm on MasterProcessor\n");
    	}
    	if(pthread_create(&pt, NULL, &midTerm, NULL)) {
    	    printf("Could not create midTerm on MasterProcessor\n");
    	}
    	if(pthread_create(&pt, NULL, &longTerm, NULL)) {
    	    printf("Could not create longTerm on MasterProcessor\n");
    	}
	}

	~MasterProcessor() {
	}

protected: 
	MasterProcessor() {
	}

private:
	void shortTerm();
	void midTerm();
	void longTerm();
	pthread_t pt;
	ReadyMLFQ &rq;
	BlockQueue &bq;
};


class SlaveProcessor {
public:
	SlaveProcessor(ReadyMLFQ &rq0, BlockQueue &bq0) {
		if(pthread_create(&pt, NULL, &running, NULL)) {
    	    printf("Could not create thread on MasterProcessor\n");
    	}
	}

private:
	void running();
	pthread_t pt;

};