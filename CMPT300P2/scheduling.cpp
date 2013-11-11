#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

using namespace std;

void Processor() {
	while(1) {
		// system kernal space (short term scheduler)
		Process pro = fetch process from ready queue;

		int whichScheduler = 0;
		
		//User space
		for (int i=0; i < runningTime(pro); i++) {
			if (pro.executeOneCommand() != BLOCK);
			else {
				whichScheduler = 1;
				break;
			}
		}

		// system kernal space
		if (whichScheduler == 0) {
			put pro into ready queue;	//short term scheduler behavior
		}
		else if (whichScheduler == 1) {
			put pro into block queue;	//mid term scheduler behavior
		}
	}
}

void midTern() { //interrupt of the block end
	while (1) {
		for (Process pro : block queue) {
			if (pro received io) {
				put pro into ready queue;
			}
		}
	}
}


void longTerm() {

}

int main() {
	return 0;
}
