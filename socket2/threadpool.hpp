#ifndef _THREADPOOL
#define _THREADPOOL
#pragma once
#include <iostream>
#include <pthread.h>
#include "Mutex.hpp"
#include "cond.hpp"
#include <queue>
#include <vector>
#include <string>

using namespace _COND_SPACE;
using namespace _MUTEX_SPACE;
namespace _THREADPOOL_SPACE
{
    template <class T>
    class threadpool
    {
    public:
        static void *threadroutine(void *argc)
        {
            threadpool<T> *pa = static_cast<threadpool<T> *>(argc);
            while(true){
                pa->_task_lock.lock();
                std::cout<<"线程执行"<<std::endl;
                while(pa->queueisempty()){
                    pa->_cond.Wait(&pa->_task_lock);
                }
                T a=pa->Gettask();
                pa->_task_lock.unlock();
                a();
            }
        }
        // std::string Getstrname()
        // {
        // }
        bool queueisempty()
        {
            bool a=pq.empty();
            return a;
        }
        threadpool(int cnt = 5)
        {
            while (cnt--)
            {
                pthread_t id;
                pthread_create(&id, nullptr, threadroutine, this);
                arr_id.push_back(id);
            }
        }
        void Enqueue(T cb)
        { // lambda表达式？？？
            _task_lock.lock();
            pq.push(cb);
            _cond.signal();
            _task_lock.unlock();
        }
        T Gettask()
        {
            T p = pq.front();
            pq.pop();
            return p;
        }

    private:
        std::vector<pthread_t> arr_id;
        std::queue<T> pq;
        Cond _cond;
        Mutex _task_lock;
        int _lowsize = 1;
        int _maxsize;
    };
}

#endif _THREADPOOL