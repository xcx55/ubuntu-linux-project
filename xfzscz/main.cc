#include "queue.hpp"

int pushscore()
{
    return rand() % 101;
}

// 消费者
void *croutine(void *argc)
{
    blackqueue<int> *p = static_cast<blackqueue<int> *>(argc);
    while (true)
    {
        int data;
        p->get(&data);
        std::cout << "消费者：分数：" << data << std::endl;
        sleep(1);
    }
}

// 生产者
void *proutine(void *argc)
{
    blackqueue<int> *p = static_cast<blackqueue<int> *>(argc);
    while (true)
    {
        int data = pushscore();
        p->push_back(data);
        std::cout << "生产者：放入分数" << data << std::endl;
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    blackqueue<int> *pq = new blackqueue<int>;
    pthread_t c, p;
    pthread_create(&c, nullptr, croutine, (void *)pq);
    pthread_create(&p, nullptr, proutine, (void *)pq);
    while(1){}
    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    return 0;
}