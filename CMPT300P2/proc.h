#ifndef PROC_H
#define PROC_H
#include "header.h"

using namespace std;

class Proc{
public:

    Proc(int id);
    Proc(int id, int pt);
    ~Proc(){};
    int isBlocked();
    int getID();
    int getPriority();
    int proc_execute();
    void setBlockState(int);
    void changePriority(int i);
	
protected:
    Proc(){};

private:
    int priority;
    int procID;
    int procType; //cpu-bounded as 2, normal as 1, io-bounded as 0
    int blockState;
    list<int> loc;   //"lines of codes"

    void initialize_loc();
};
#endif

