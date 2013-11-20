#ifndef PROC_H
#define PROC_H
#include "header.h"

class Proc{
public:

    Proc(int id);
    Proc(int id, int pt);
    ~Proc();
    bool isBlocked();
    bool isRunning();
    int getID();
    int getPriority();
    int proc_execute();
    void setState(int s);
    void changePriority(int i);
	
protected:
    Proc(){};

private:
    int priority;
    int procID;
    int procType; //cpu-bounded as 2, normal as 1, io-bounded as 0
    int state;
    std::list<int> loc;   //"lines of codes"
	pthread_mutex_t state_mutex;
    void initialize_loc();
};
#endif

