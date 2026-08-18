#include <queue>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
const int thefullcapacity = 6;
template <class T>
class blackqueue
{
public:
    blackqueue()
        : pq(new std::queue<T>), cap(6)
    {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cost, nullptr);
        pthread_cond_init(&product, nullptr);
    }
    ~blackqueue()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cost);
        pthread_cond_destroy(&product);
    }
    bool isfull()
    {
        return pq->size >= cap;
    }
    void push_back(T &in)
    {
        pthread_mutex_lock(&mutex);
        if (pq->size() >= costlower)
        {
            pthread_cond_broadcast(&cost);
            pthread_cond_wait(&product, &mutex);
        }

        pq->push(in);

        pthread_mutex_unlock(&this->mutex);
    }
    void get(T *out)
    {
        pthread_mutex_lock(&mutex);
        if (pq->size() <= productlower)
        {
            pthread_cond_broadcast(&product);
            pthread_cond_wait(&cost, &mutex);
        }

        *out = pq->front();
        pq->pop();
        pthread_mutex_unlock(&mutex);
    }

private:
    std::queue<T> *pq;
    int cap;
    pthread_mutex_t mutex;
    pthread_cond_t cost;
    pthread_cond_t product;
    int productlower=1;
    int costlower=5;
};