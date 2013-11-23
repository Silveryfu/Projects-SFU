#include "masterprocessor.h"
#include "slaveprocessor.h"

using namespace std;

int main() {
	srand(time(NULL));
	int proc_pip[SLAVES_NUMBER][2];
	int idle_pip[SLAVES_NUMBER][2];
	for (int i=0; i<SLAVES_NUMBER; i++) {
		pipe(proc_pip[i]);
		pipe(idle_pip[i]);
	}
	ReadyMLFQ *rq = new ReadyMLFQ();
	
	BlockQueue *bq = new BlockQueue();

	MasterProcessor *MsP = new MasterProcessor(rq, bq, proc_pip, idle_pip);

	SlaveProcessor *SlP[SLAVES_NUMBER];
	for (int i=0; i<SLAVES_NUMBER; i++)
		SlP[i] = new SlaveProcessor(rq, bq, proc_pip[i], idle_pip[i], i+1);

	pthread_exit(NULL);

	delete rq;
	delete bq;
	delete MsP;
	for (int i=0; i<SLAVES_NUMBER; i++) delete SlP[i];
	
	return 0;
}
