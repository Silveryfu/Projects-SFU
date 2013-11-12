#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

using namespace std;

int const NO_INTTERUPT = 1;
int const MID_TERM_TO_BQ = 2;
int const MID_TERM_BQ_TO_RQ = 3;
int const LONG_TERM_TO_RQ = 4;
int const LONG_TERM_EXIT = 5;

void masterProcessor() {
	while(1) {
		// system kernal space (short term scheduler)
		Process pro = getProcessFromRQ();
		while ( thereIsIdleProcessor() ) {
			putProToIdleProcessor(pro);
			pro = getProcessFromRQ();
		}
		int whichScheduler = NO_INTTERUPT;
		
		//User space
		for (int i=0; i < runningTime(pro); i++) {
			if ( (whichScheduler=pro.executeOneCommand()) != NO_INTTERUPT) break;
			if ( (whichScheduler=longTermIntterupt()) != NO_INTTERUPT ) break;
			if ( (whichScheduler=midTermIntterupt()) != NO_INTTERUPT) break;
		}

		// system kernal space
		switch (whichScheduler) {
			case NO_INTTERUPT: {
				putProcessToRQ(pro);
			}; break;

			case MID_TERM_TO_BQ: {
				putProcessToBQ(pro);
			}; break;

			case MID_TERM_BQ_TO_RQ: {
				putProcessToRQ(pro);
				mvProFromBQtoRQ();
			}; break;

			case LONG_TERM_TO_RQ: {
				putProcessToRQ(pro);
				dealWithNewProcess();
			}; break;

			case LONG_TERM_EXIT: {
				deleteThisProcess(pro);
			}; break;
		}

	}
}

void slaveProcessor() {

}

int main() {
	return 0;
}
