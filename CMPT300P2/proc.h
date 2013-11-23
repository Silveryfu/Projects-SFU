#ifndef PROC_H
#define PROC_H
#include "monitor.h"

class Proc{
public:

    Proc(int);
    Proc(int, int);
    ~Proc();
    bool isBlocked();
    bool isRunning();
    int getID();
    int getPriority();
    int proc_execute();
    int restCommands();
    void setState(int);
    void changePriority(int);
    int getBlocTime();
    void setBlocTime(int);
protected:
    Proc(){};

private:
    int priority;
    int procID;
    int procType; //cpu-bounded as 2, normal as 1, io-bounded as 0
    int state;  //IPC issue involved on this variable
    int blocTime;
	std::list<int> loc;   //"lines of codes"
	pthread_mutex_t state_mutex;
    void initialize_loc();
};

//This class wrap process and running time together, in order to pass those info to slave processor
class ProcWrapper{
public:
    ProcWrapper(Proc *pro0, int timeQuanta0) {
        pro = pro0;
        timeQuanta = timeQuanta0;
    }
    Proc *pro;
    int timeQuanta;
protected:
    ProcWrapper(){};
};

#endif

