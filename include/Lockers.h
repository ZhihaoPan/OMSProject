#ifndef LOCKERS_H
#define LOCKERS_H

#include <pthread.h>
#include <exception>
#include <spdlog/spdlog.h>
#include <semaphore.h>

class sem
{
public:
    sem()
    {
        if (sem_init(&m_sem, NULL, NULL) != 0)
        {
            spdlog::error("[LOCKER] semphore init failed.");
            throw std::exception();
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem);
    }
    bool wait()
    {
        return sem_wait(&m_sem) == 0;
    }
    bool post()
    {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};

class locker
{
public:
    locker()
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            spdlog::error("[LOCKER] locker init failed");
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

private:
    pthread_mutex_t m_mutex;
};

class cond
{
public:
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            spdlog::error("[LOCKER] cond init failed.");
            throw std::exception();
        }
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            pthread_cond_destroy(&m_cond);
            spdlog::error("[LOCKER] mutex init failed in cond construct.");
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, &m_mutex);
        pthread_mutex_unlock(&m_mutex);
        return ret;
    }

    bool notify()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
};

#endif