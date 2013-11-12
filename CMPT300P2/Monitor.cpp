#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define synchronized(M) for(Lock M##_lock=M; M##_lock; M##_lock.setUnlock())

using namespace std;

int count=30;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

class Lock
{
public:
	Lock(pthread_mutex_t &mutex):m_mutex(mutex),m_locked(true)
	{
		pthread_mutex_lock(&mutex);
	}

	~Lock()
	{
	 	pthread_mutex_unlock(&m_mutex);
	}

	operator bool() const
	{
		return m_locked;
	}

	void setUnlock()
	{
		m_locked = false;
	}

private:
	pthread_mutex_t &m_mutex;
        bool m_locked;
};	

void *read_and_print(void *para){
	synchronized(mutex){
	struct timespec ts;
	ts.tv_sec;
	ts.tv_nsec=0;
	while(count>0){
		cout<<"-Count is "<<count<<"This is \n"<<(int) para<<endl;	
		pthread_yield();	
		count--;
	      }
	}
}



int main(){
	cout<<"How is it going?"<<endl;
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,read_and_print,(void*)1);
	pthread_create(&tid2,NULL,read_and_print,(void*)2);
	pthread_exit(NULL);
	return 0;
}
