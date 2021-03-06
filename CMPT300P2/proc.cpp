#include "proc.h"

Proc::Proc(int id){
    priority=LEVEL;    //priority is initialized to LEVEL
    procID=id;
    blocTime=0;
    procType=1;    //normal as default
    /*generate the simulation of a process and IOs*/
    initialize_loc();
    pthread_mutex_init(&state_mutex, NULL);
    setState(PROC_CREATE);
};

Proc::Proc(int id, int pt){
    priority=LEVEL;
    procID=id;
    procType=pt;
    /*generate the simulation of a process and IOs*/
    initialize_loc();
    pthread_mutex_init(&state_mutex, NULL);
    setState(PROC_CREATE);
};

Proc::~Proc(){pthread_mutex_destroy(&state_mutex);}

int Proc::getBlocTime(){
	return blocTime;
}

void Proc::setBlocTime(int b){
	blocTime=b;
}

bool Proc::isBlocked(){
    bool isBlock;
    synchronized(state_mutex){
        isBlock =(state == PROC_BLOCK);
    }
    return isBlock;
}

bool Proc::isRunning() {
    bool isRun;
	synchronized(state_mutex){
	   isRun=(state!=PROC_EXIT);
	}
    return isRun;
}

int Proc::getID(){
    return procID;
}

int Proc::getPriority(){
    return priority;
}

void Proc::setState(int s) {
	synchronized(state_mutex){
    state = s;    
	}
}

void Proc::changePriority(int i){
    priority+=i;
};

void Proc::initialize_loc(){
    int num_of_lines=MIN_NUM_OF_CODE+rand()%(MAX_NUM_OF_CODE - MIN_NUM_OF_CODE); //a process will have 10 to 110 lines of code
    int io_odd;  //probablity of a io occurence
    for(int i=0;i<num_of_lines;i++){
        io_odd=rand()%(procType*30+5);   //averagely, an io-bounded proc io occurs 1 out of 5, normal: 1/35, cpu-bounded: 1/65;
        if(io_odd!=0) 
            loc.push_front(PROC_RUN);
        else loc.push_front(PROC_BLOCK);
    }
}

int Proc::proc_execute(){
    if(loc.size()==0) return PROC_EXIT;
    else{
        int proc_state=loc.front();
        loc.pop_front();
        return proc_state;
    }
}

int Proc::restCommands() {
    return loc.size();
}
