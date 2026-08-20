#ifndef _BLACKQUEUE
#define _BLACKQUEUE
#include <queue>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include"cond.hpp"
const int thefullcapacity = 6;
template <class T>
class blackqueue
{
public:
    blackqueue()
        : pq(new std::queue<T>), cap(6)
    {
        pthread_mutex_init(&mutex, nullptr);
        // pthread_cond_init(&cost, nullptr);
        // pthread_cond_init(&product, nullptr);
    }
    ~blackqueue()
    {
        pthread_mutex_destroy(&mutex);
        // pthread_cond_destroy(&cost);
        // pthread_cond_destroy(&product);
    }
    bool isfull()
    {
        return pq->size >= cap;
    }
    void push_back(T &in)
    {
        pthread_mutex_lock(&mutex);//
        while (pq->size() >= costlower)
        {
            // pthread_cond_signal(&cost);
            cost.signalcond();
            product.waitcond(this);
            // pthread_cond_wait(&product, &mutex);//防止函数失败！
        }

        pq->push(in);

        pthread_mutex_unlock(&this->mutex);
    }
    void get(T *out)
    {
        pthread_mutex_lock(&mutex);
        while (pq->size() <= productlower)
        {
            // pthread_cond_signal(&product);
            product.signalcond();
            // pthread_cond_wait(&cost, &mutex);
            cost.waitcond(this);
        }

        *out = pq->front();
        pq->pop();
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t* Getmutex(){
        return &mutex;
    }

private:
    std::queue<T> *pq;
    int cap;
    pthread_mutex_t mutex;
    cond<T> cost;
    cond<T> product;
    // pthread_cond_t cost;
    // pthread_cond_t product;
    int productlower=1;
    int costlower=5;
};

#endif 