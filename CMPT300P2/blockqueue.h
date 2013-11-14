#include "monitor.h"
#include "proc.h"

int const NO_PROCESS_NONBLOCKED=1;

class BlockedQueue{
private:
    pthread_mutex_t blockedQueueMutex;
    std::list<Proc> blockedQueue;
public:
    BlockedQueue(){
        blockedQueueMutex=PTHREAD_MUTEX_INITIALIZER;
        blockedQueue.clear();
    }

    int checkIO(Proc *procPtr){
        synchronized(blockedQueueMutex){
            std::list<Proc>::iterator iter;
            for(iter=blockedQueue.begin();iter!=blockedQueue.end();iter++){
                Proc temp=*iter;
                if(!temp.isBlocked()){
                    blockedQueue.erase(iter);
                    *procPtr=temp;
                    return 0;
                }
            }
            return NO_PROCESS_NONBLOCKED;
        }
    }

    void putProc(Proc process){
        synchronized(blockedQueueMutex){
            blockedQueue.push_back(process);
        }
    }
};
