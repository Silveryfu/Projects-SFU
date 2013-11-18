#include "processor.h"

/*This file consists of both master processor's and slave processor's
 *definition.*/

/* ---------------------- MASTER PROCESSOR------------------------- */

MasterProcessor::MasterProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int proc_pip0[][2], int idle_pip0[][2]){
	rq = rq0;
	bq = bq0;
	proc_pip = proc_pip0;
	idle_pip = idle_pip0;
   	if(pthread_create(&pt[0], NULL, &runShortTermScheduler, (void*)this)) {  //Create short-term scheduler as a thread
   	    printf("Could not create shortTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[1], NULL, &runMidTermScheduler, (void*)this)) {  //Create mid-term scheduler as a thread
   	    printf("Could not create midTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[2], NULL, &runLongTermScheduler, (void*)this)) {  //Create long-term scheduler as a thread
   	    printf("Could not create longTerm on MasterProcessor\n");
   	}
	all_processes.clear();
   	pthread_exit(NULL);
}

void MasterProcessor::shortTermScheduler() {
	//This array is to store the wrap object from class ProcAndTime for each slave processor
	ProcWrapper * pw[SLAVES_NUMBER];
	for (int i=0; i<SLAVES_NUMBER; i++) {
		//Setting non block for reading the idle_pipe
		if ( fcntl(idle_pip[i][0], F_SETFL, O_NONBLOCK) == -1) printf("non block fail on slave %d\n",i);
		pw[i] = NULL;
	}

	while (1) {
		Proc *pro;
		pro = rq->getProc(); //Get a process from ready queue
		if (pro != NULL) {	//If there still at least a process in ready queue   /******may busy waiting /******Is there a better way?
			for (int i=0; i<SLAVES_NUMBER; i++) {
				bool isIdle = false;
				read(idle_pip[i][0], &idle, sizeof(bool)); //non-block reading the idle_pipe
				if ( isIdle ) {	//If the slave is idle
					if (pw[i] != NULL) {
						delete pw[i];	//In case of memory leak
						pw[i] = NULL;
					}
					pw[i] = new ProcWrapper(pro, TIME_UNIT * pro->getPriority());
					write(proc_pip[i][1], &pw[i], sizeof(ProcWrapper *));
				}
			}
		}
	}
}

void MasterProcessor::midTermScheduler() {
	srand(time(NULL));
	while (1) {
		vector<Proc *> bqV = bq->getList();
		int length = bqV.size();
		int index = rand() % (length * IO_WAIT_TIME);
		if (index < length) {
			bqV[index]->setBlockState(PROC_RUN); //IO blocking ends
			Proc *pro = bq->checkIO();
			if (pro != NULL) rq->putProc(pro);
		}
	}
}

void MasterProcessor::longTermScheduler() {
	srand(time(NULL));
	while (1) {
		sleep(1.0/CREATE_PROC_FREQUENCY);
		int proc_id=-1;
		for (int i=0; i < (int)all_processes.size(); i++) { //delete the exited process and collect the proc_id
			if (!all_processes[i]->isRunning()) {
				delete all_processes[i];
				proc_id = i;
				break;
			}
		}

		Proc *pro;
		if (proc_id == -1) { //If no previous process exited
			proc_id = (int)all_processes.size();
			pro = new Proc(proc_id);
			all_processes.push_back(pro);
		}
		else { //When there is a previous process exited, make the new process
			pro = new Proc(proc_id);
			all_processes[i] = pro;
		}
		
		rq->putProc(pro);
		if (proc_id > MAX_PROCESS_NUMBER) { //When creating too many processes, sleep for a while
			sleep(10);
		}
	}
}

/* -------------------- SLAVE PROCESSOR ---------------------- */

SlaveProcessor::SlaveProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int *s_proc_pip0, int *s_idle_pip0) {
	rq = rq0;
	bq = bq0;
	s_proc_pip = s_proc_pip0;
	s_idle_pip = s_idle_pip0;
	if(pthread_create(&pt, NULL, &run, (void*)this)) {
   	    printf("Could not create running thread on SlaveProcessor\n");
   	}
}

void SlaveProcessor::running() {
	while (1) {
		ProcWrapper *pw;
		bool const isIdle = true;
		write(s_idle_pip[1], &isIdle, sizeof(bool)); //Write back the idle signal to idle_pipe
		read(s_proc_pip[0], &pw, sizeof(ProcWrapper *));  //read the process_pipe from short-term scheduler, this will block if the pipe is empty

		int proc_state = PROC_RUN;
		for (int i=0; i<pw->timeQuanta; i++) {
			proc_state = pw->pro->proc_execute();
			if (proc_state == PROC_BLOCK || proc_state == PROC_EXIT) break;
		}

		switch(proc_state) {
		case PROC_BLOCK: //Process IO Block
			pw->pro->setState(PROC_BLOCK);
			bq->putProc(pw->pro);
		    break;
		case PROC_EXIT://Process finish executing and exit
			cout<<"A process exits."<<endl;
			pw->pro->setState(PROC_EXIT);
		    break;
		case PROC_RUN://use up the time quanta but not finishes
		default:  
			if ( pw->pro->getPriority() < LEVEL ) pw->pro->changePriority(1);
			rq->putProc(pw->pro);
		    break;
		}
	}

}


