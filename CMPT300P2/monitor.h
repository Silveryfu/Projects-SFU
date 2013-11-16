#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#define synchronized(M) for(Lock M##_lock=M; M##_lock; M##_lock.setUnlock())

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
#endif