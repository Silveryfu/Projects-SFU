#include "monitor.h"
#include "proc.h"

int const NO_PROCESS_NONBLOCKED=1;

class BlockQueue{
private:
    pthread_mutex_t blockQueueMutex;
    std::list<Proc> blockQueue;
public:
    BlockQueue();

    int checkIO(Proc *procPtr);

    void putProc(Proc process);
};
