#include "proc.h"

Proc::Proc(int id){
    priority=LEVEL;    //priority is initialized to LEVEL
    procID=id;
    state=PROC_RUN;
    procType=1;    //normal as default
    /*generate the simulation of a process and IOs*/
    initialize_loc();
};

Proc::Proc(int id, int pt){
    priority=LEVEL;
    procID=id;
    state=PROC_RUN;
    procType=pt;
    /*generate the simulation of a process and IOs*/
    initialize_loc();
};

bool Proc::isBlocked(){
    return state == PROC_BLOCK;
}

bool Proc::isRunning() {
    return state != PROC_EXIT;
}

int Proc::getID(){
    return procID;
}

int Proc::getPriority(){
    return priority;
}

void Proc::setState(int s) {
    state = s;
}

void Proc::changePriority(int i){
    priority+=i;
};

void Proc::initialize_loc(){
    srand(time(NULL));
    int num_of_lines=10+rand()%100;   //a process will have 10 to 110 lines of code
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
