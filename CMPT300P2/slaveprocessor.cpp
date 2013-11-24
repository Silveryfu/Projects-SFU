#include "slaveprocessor.h"

SlaveProcessor::SlaveProcessor(ReadyMLFQ *rq0, BlockQueue *bq0, int *s_proc_pip0, int *s_idle_pip0, int slaveID0) {
	rq = rq0;
	bq = bq0;
	s_proc_pip = s_proc_pip0;
	s_idle_pip = s_idle_pip0;
	if(pthread_create(&pt, NULL, &run, (void*)this)) {
   	    printf("Could not create running thread on SlaveProcessor\n");
   	}
   	slaveID = slaveID0;
   	for (int i=0; i<slaveID*SLAVE_INDENT_WIDTH; i++) indent[i] = '\t';
}

void SlaveProcessor::running() {
	while (1) {
		ProcWrapper *pw;
		bool const isIdle = true;
		write(s_idle_pip[1], &isIdle, sizeof(bool)); //Write back the idle signal to idle_pipe
		read(s_proc_pip[0], &pw, sizeof(ProcWrapper *));  //read the process_pipe, this will block if the pipe is not yet been filled things by short-term-scheduler

		int proc_state = PROC_RUN;
		
		printf("%s(%d) Process(PID=%d) executing\n%s    Commands to be run:%d\n",indent, slaveID, pw->pro->getID(), indent, pw->pro->restCommands());
		for (int i=0; i<pw->timeQuanta; i++) {
			proc_state = pw->pro->proc_execute();
			if (proc_state == PROC_BLOCK || proc_state == PROC_EXIT) break;
		}

		switch(proc_state) {
		case PROC_BLOCK: //Process IO Block
			printf("%s(%d) Process(PID=%d) IO-Block\n%s    Put in BlockQueue\n%s    Commands to be run:%d\n",indent, slaveID, pw->pro->getID(), indent, indent, pw->pro->restCommands());
			pw->pro->setState(PROC_BLOCK);
			pw->pro->setBlocTime(1+rand()%100);
			bq->putProc(pw->pro);
		    break;
		case PROC_EXIT://Process finish executing and exit
			printf("%s(%d) Process(PID=%d) exits\n",indent, slaveID, pw->pro->getID());
			pw->pro->setState(PROC_EXIT);
		    break;
		case PROC_RUN://use up the time quanta but not finishes
		default:  
			if ( pw->pro->getPriority() > 1 ) pw->pro->changePriority(-1);
			printf("%s(%d) Process(PID=%d) swapped out\n%s    Changed priority to %d\n%s    Back to ReadyQueue\n%s    Commands to be run:%d\n",indent, slaveID, pw->pro->getID(), indent, pw->pro->getPriority(), indent, indent, pw->pro->restCommands());
			rq->putProc(pw->pro);
		    break;
		}
	}

}

