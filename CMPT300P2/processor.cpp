#include "header.h"

/*This file consists of both master processor's and slave processor's
 *definition.*/

/* ---------------------- MASTER PROCESSOR------------------------- */

MasterProcessor::MasterProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int **proc_pip0, int **idle_pip0){
	rq = rq0;
	bq = bq0;
	proc_pip = proc_pip0;
	idle_pip = idle_pip0;
   	if(pthread_create(&pt[0], NULL, &shortTerm, NULL)) {  //Create short-term scheduler as a thread
   	    printf("Could not create shortTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[1], NULL, &midTerm, NULL)) {  //Create mid-term scheduler as a thread
   	    printf("Could not create midTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[2], NULL, &longTerm, NULL)) {  //Create long-term scheduler as a thread
   	    printf("Could not create longTerm on MasterProcessor\n");
   	}
	all_processes.clear();
   	pthread_exit(NULL);
}

void MasterProcessor::shortTerm() {
	//This array is to store the wrap object from class ProcAndTime for each slave processor
	ProcWrapper * pw[SLAVES_NUMBER];
	for (int i=0; i<SLAVES_NUMBER; i++) {
		//Setting non block for reading the idle_pipe
		if ( fcntl(idle_pip[0], F_SETFL, O_NONBLOCK) == -1) printf("non block fail on slave %d\n");
		pw[i] = NULL;
	}

	while (1) {
		Proc *pro;
		pro = rq.getProc(); //Get a process from ready queue
		if (pro != NULL) {	//If there still at least a process in ready queue   /******may busy waiting
			for (int i=0; i<SLAVES_NUMBER; i++) {
				int idle = 0;
				read(idle_pip[i][0], &idle, sizeof(int)); //non-block reading the idle_pipe
				if ( idle == 1 ) {	//If the slave is idle
					if (pw[i] != NULL) {
						delete pw[i];
						pw[i] = NULL;
					}
					pw[i] = new ProcWrapper(pro, TIME_UNIT * pro->getPriority());
					write(proc_pip[i][1], &pw[i], sizeof(ProcWrapper *));
				}
			}
		}
	}
}

void MasterProcessor::midTerm() {
	srand(time(NULL));
	while (1) {
		vector<Proc *> bqV = bq.getList();
		int length = bqV.size();
		int index = rand() % (length * IO_WAIT_TIME);
		if (index < length) {
			bqV[index].setBlockState(0);
			Proc * pro = bq.checkIO();
			if (pro == NULL) rq.putProc(pro);
		}
	}
}

void MasterProcessor::longTerm() {
	srand(time(NULL));
	int proc_id=1;
	while (1) {
		sleep(1.0/CREATE_PROC_FREQUENCY);
		Proc *pro = new Proc(proc_id);
		all_processes.push_back(pro);
		rq.putProc(pro);
		proc_id++;
		if (proc_id > MAX_PROCESS_NUMBER) { //When creating too much processes, sleep for a while, and cut the i to its half
			sleep(10);
		} 
	}
}

/* -------------------- SLAVE PROCESSOR ---------------------- */

SlaveProcessor::SlaveProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int *s_proc_pip0, int *s_idle_pip0) {
	rq = rq0;
	bq = bq0;
	s_proc_pip = s_proc_pip0;
	s_idle_pip = s_idle_pip0;
	if(pthread_create(&pt, NULL, &running, NULL)) {
   	    printf("Could not create running thread on SlaveProcessor\n");
   	}
}

void SlaveProcessor::running() {
	while (1) {
		ProcWrapper *pw;
		int const idle = 1;
		write(s_idle_pip[1], &idle, sizeof(int)); //Write back the idle signal to idle_pipe
		read(s_proc_pip[0], &pw, sizeof(ProcWrapper *));  //read the process_pipe from short-term scheduler, this will block if the pipe is empty

		int proc_state = PROC_RUN;
		for (int i=0; i<pw->timeQuanta; i++) {
			proc_state = pw->proc_execute();
			if (proc_state == PROC_EXIT || proc_state == PROC_BLOCK) break;
		}

		switch(proc_state) {
			case PROC_BLOCK: //Process IO Block
				pw->pro->setBlockState(1);
				bq.putProc(pw->pro);
			        break;
			case PROC_EXIT://Process finish executing and exit
				cout<<"A process exits."<<endl;
			        break;
			default:  //use up the time quanta but not finishes
				if ( pw->pro->getPriority() < LEVEL ) pw->pro->changePriority(1);
				else pw->pro->changePriority(0);
				rq.putProc(pw->pro);
			        break;
			}
}


