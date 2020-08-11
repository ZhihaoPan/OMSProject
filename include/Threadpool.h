#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <string>
#include <spdlog/spdlog.h>
#include <exception>
#include <pthread.h>
#include "Lockers.h"

template<typename T>
class ThreadPool
{
public:
    ThreadPool(int thread_num = 8, int max_request = 10000);
    ~ThreadPool();

    int append(T *request);
private:
    static int worker(void *);
    int run();

private:
    int m_thread_num;
    int m_max_request;
    pthread_t* m_threads;
    std::list<T*> m_workqueue;
    locker m_queuelocker;
    sem m_queuesem;
    bool m_stop;
};

template<typename T>
ThreadPool<T>::ThreadPool(int thread_num, int max_request) 
                    : m_thread_num(thread_num), m_max_request(max_request), 
                    m_threads(NULL), m_stop(false)
{
    if (thread_num <= 0 || max_request <= 0)
    {
        spdlog::error("[THREADPOOL] threadnum <= 0 or max_request <= 0.");
        throw std::exception();
    }
    
    m_threads = new pthread_t[m_thread_num];
    if (!m_threads)
    {
        spdlog::error("[THREADPOOL] new thread failed.");
        throw std::exception();
    }

    for (int i = 0; i < m_thread_num; i++)
    {
        if (pthread_create(&m_threads[i], NULL, worker, this) != 0)
        {
            spdlog::error("[THREADPOOL] create thread : " + std::to_string(i) + "failed.");
            delete [] m_threads;
            throw std::exception();
        }
        // 将子线程和父线程进行脱离，保证父线程退出后子线程不会被自动退出
        // 子线程中需要自行处理线程退出
        if (pthread_detach(m_threads[i]))
        {
            delete []m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
ThreadPool<T>::~ThreadPool()
{
    delete []m_threads;
    m_stop = true;
}

template<typename T>
int ThreadPool<T>::append(T *request)
{
    m_queuelocker.lock();
    if (m_workqueue.size() > m_max_request)
    {
        m_queuelocker.unlock();
        return -1;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuesem.post();
}

template<typename T>
int ThreadPool<T>::worker(void *arg)
{
    ThreadPool* pool = (ThreadPool*) arg;
    pool->run();
    return 0;
}

template<typename T>
int ThreadPool<T>::run()
{
    while (!m_stop)
    {
        m_queuesem.wait();
        m_queuelocker.lock();
        if (m_wokequeue.size() == 0)
        {
            m_queuelocker.unlock();
            continue;
        }   
        T* req = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelock.unlock();
        if (!req)
        {
            continue;
        }
        req->Task();
    }
    
    return 0;
}

#endif