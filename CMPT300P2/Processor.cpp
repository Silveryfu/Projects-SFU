#include "processors.h"

// ---------------------- MASTER -------------------------

MasterProcessor::MasterProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int **proc_pip0, int **idle_pip0);
	rq = rq0;
	bq = bq0;
	proc_pip = proc_pip0;
	idle_pip = idle_pip0;
   	if(pthread_create(&pt[0], NULL, &shortTerm, NULL)) {
   	    printf("Could not create shortTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[1], NULL, &midTerm, NULL)) {
   	    printf("Could not create midTerm on MasterProcessor\n");
   	}
   	if(pthread_create(&pt[2], NULL, &longTerm, NULL)) {
   	    printf("Could not create longTerm on MasterProcessor\n");
   	}

}

void MasterProcessor::shortTerm() {
	ProcAndTime * pats[SLAVES_NUMBER];
	for (int i=0; i<SLAVES_NUMBER; i++) {
		//Setting non block
		if ( fcntl(idle_pip[0], F_SETFL, O_NONBLOCK) == -1) printf("non block fail on slave %d\n");
		pats[i] = NULL;
	}

	while (1) {
		Proc *pro;
		pro = rq.getProc();
		if (pro != NULL) {
			for (int i=0; i<SLAVES_NUMBER; i++) {
				int idle = 0;
				read(idle_pip[i][0], &idle, sizeof(int));
				if ( idle == 1 ) {
					if (pat[i] != NULL) {
						delete pat[i];
						pat[i] = NULL;
					}
					pat[i] = new ProcAndTime(pro, TIME_UNIT * pro->getPriority());
					write(proc_pip[i][1], &pats[i], sizeof(ProcAndTime *));
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
	int i=1;
	srand(time(NULL));
	while (1) {
		sleep(0.3);
		Proc *pro = new Proc(rand()%LEVEL + 1, i);
		rq.putProc(pro);
		if (i > MAX_PROCESS_NUMBER) { //When creating too much processes, sleep for a while, and cut the half of i
			sleep(10);
			i /= 2;
		} 
	}
}

// -------------------- SLAVES ----------------------

SlaveProcessor::SlaveProcessor(ReadyMLFQ &rq0, BlockQueue &bq0, int *s_proc_pip0, int *s_idle_pip0) {
	rq = rq0;
	bq = bq0;
	s_proc_pip = s_proc_pip0;
	s_idle_pip = s_idle_pip0;
	if(pthread_create(&pt, NULL, &running, NULL)) {
   	    printf("Could not create longTerm on MasterProcessor\n");
   	}
}

void SlaveProcessor::running() {
	while (1) {
		ProcAndTime *pat;
		int const idle = 1;
		write(s_idle_pip[1], &idle, sizeof(int)); //Block Write
		read(s_proc_pip[0], pat, sizeof(ProcAndTime));  //Block Read

		int which = 1;
		for (int i=0; i<pat->timeQuan; i++) {
			which = pat->proc_execute();
			if (which == -1 || which == 0) break;
		}

		switch(which) {
			case 0: {  //Process IO Block
				pat->pro->setBlockState(1);
				bq.putProc(pat->pro);
			} break;
			case -1: { //Process finish executing and exit

			} break;
			default: if (which > 0) { //Normal switch of short term
				if ( pat-pro->getPriority() < LEVEL ) pat->pro->changePriority(1);
				else pat->pro->changePriority(0);
				rq.putProc(pat->pro);
			} break;
		}

	}
}

