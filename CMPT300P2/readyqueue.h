#include "monitor.h"
#include "proc.h"

int const NO_PROCESS_IN_QUEUE=1;

class ReadyMLFQ{
private:
    pthread_mutex_t readyMLFQMutex;
    std::queue<Proc> readMLFQ[3];
public:
    ReadyMLFQ(){
        readyMLFQMutex=PTHREAD_MUTEX_INITIALIZER;
        for(int i=0;i<3;i++){
            while(!readMLFQ[i].empty()){
                readMLFQ[i].pop();
            }
        }
    }

    void putProc(Proc process){
        synchronized(readyMLFQMutex){
            readMLFQ[process.getPriority()-1].push(process);
        }
    }

    int getProc(Proc *procPtr){
        synchronized(readyMLFQMutex){
            for(int i=0;i<3;i++){
                if(!readMLFQ[i].empty()){
                    *procPtr=readMLFQ[i].front();
                    readMLFQ[i].pop();
                    return 0;
                }
            }
            return NO_PROCESS_IN_QUEUE;
        }
    }

    int size(){
        return readMLFQ[0].size()+readMLFQ[1].size()+readMLFQ[2].size();
    }
};
