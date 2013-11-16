#ifndef PROC_H
#define PROC_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>

using namespace std;

class Proc{
public:

    Proc(int p, int id);
    Proc(int p, int id, int pt);
    ~Proc(){};
    int isBlocked();
    int getID();
    int getPriority();
    void proc_execute();
	
protected:
    Proc(){};

private:
    int priority;
    int procID;
    int procType; //cpu-bounded as 2, normal as 1, io-bounded as 0
    int blockState;
    std::list<int> loc;   //"lines of codes"

    void initialize_loc();
};
#endif
