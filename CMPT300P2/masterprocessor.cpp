#include "masterprocessor.h"

/*This file consists of the master processor's short-, mid- and long-term
 *schedulers' definition.
 */

/* ---------------------- MASTER PROCESSOR------------------------- */

MasterProcessor::MasterProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int proc_pip0[][2], int idle_pip0[][2]){
	rq = rq0; //Initialized the pointer of ready queue
	bq = bq0; //Initialized the pointer of block queue
	proc_pip = proc_pip0;	//initialized the pointer of pipe for passing process
	idle_pip = idle_pip0;	//initialized the pointer of pipe for telling idle situation
	while(!IDSpace.empty()){
		IDSpace.pop();
	}
	for (int i=0; i<MAX_PROCESS_NUMBER; i++) IDSpace.push(i+1);

   	if(pthread_create(&pt[0], NULL, &runShortTermScheduler, (void*)this)) {  //Create short-term scheduler as a thread
   	    printf("Could not create shortTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[1], NULL, &runMidTermScheduler, (void*)this)) {  //Create mid-term scheduler as a thread
   	    printf("Could not create midTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[2], NULL, &runLongTermScheduler, (void*)this)) {  //Create long-term scheduler as a thread
   	    printf("Could not create longTerm on MasterProcessor\n");
   	}
}

void MasterProcessor::shortTermScheduler() {
	//This array is to store the wrap object from class ProcWrapper for each slave processor
	ProcWrapper * pw[SLAVES_NUMBER];
	for (int i=0; i<SLAVES_NUMBER; i++) {
		//Setting non block for reading the idle_pipe
		if ( fcntl(idle_pip[i][0], F_SETFL, O_NONBLOCK) == -1) printf("non block fail on slave %d\n",i);
		pw[i] = NULL;
	}

	while (1) {
		for (int i=0; i<SLAVES_NUMBER; i++) {
			bool isIdle = false;
			read(idle_pip[i][0], &isIdle, sizeof(bool)); //non-block reading the idle_pipe
			if ( isIdle ) {	//If the slave is idle
				Proc *pro;
				pro = rq->getProc(); //Get a process from ready queue, will block if there is no process in ready queue
				if (pw[i] != NULL) {
					delete pw[i];	//In case of memory leak
					pw[i] = NULL;
				}
				pw[i] = new ProcWrapper(pro, TIME_UNIT * (1 << (LEVEL - pro->getPriority() + 1)) );
				printf("## Short-Term-Scheduler ##:\n  Process(PID=%d,Priority=%d)\n  Moved out from ReadyQueue\n  with timeQuanta=%d\n  Assigned to Processor(%d)\n", pw[i]->pro->getID(), pw[i]->pro->getPriority(), pw[i]->timeQuanta, i+1);
				write(proc_pip[i][1], &pw[i], sizeof(ProcWrapper *));
			}

		}
	}
}

void MasterProcessor::midTermScheduler() {
	while (1) {
			Proc* pro=bq->getProc();
			if(pro != NULL){		
				printf("## Mid-Term-Scheduler ##:\n  Process(PID=%d) IO-Block ends\n  Moved out from BlockQueue\n  Put in ReadyQueue\n", pro->getID());
				rq->putProc(pro);
				pro->setState(PROC_READY);
			}
		}
}

void MasterProcessor::longTermScheduler() {
	while (1) {
		for (list<Proc *>::iterator it=all_processes.begin(); it!=all_processes.end();) {
			if ( !(*it)->isRunning() ) {
				printf("## Long-Term-Scheduler ##:\n  Process(PID=%d) is deleted\n", (*it)->getID());
				IDSpace.push((*it)->getID());
				delete *it;
				it = all_processes.erase(it);
			}
			else {
				it++;
			}
		}

		if (IDSpace.empty()) {
			//continue;
			sleep(FOR_A_WHILE);
		}
		else {
			Proc * pro = new Proc(IDSpace.front());  
			IDSpace.pop();
			all_processes.push_back(pro);
			printf("## Long-Term-Scheduler ##:\n  Process(PID=%d) is created\n  Put in ReadyQueue\n", pro->getID());
			rq->putProc(pro);
			pro->setState(PROC_READY);
		}
	}
}

