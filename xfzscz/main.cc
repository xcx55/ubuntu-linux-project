#include "queue.hpp"
int num = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2=PTHREAD_MUTEX_INITIALIZER;
int pushscore()
{
    return rand() % 101;
}
int Getnameid()
{
    pthread_mutex_lock(&lock);
    ++num;
    pthread_mutex_unlock(&lock);
    return num;
}
// 消费者
void *croutine(void *argc)
{
    int num=Getnameid();
    blackqueue<int> *p = static_cast<blackqueue<int> *>(argc);
    std::string name="thread-"+std::to_string(num);
    while (true)
    {
        int data;
        p->get(&data);
        pthread_mutex_lock(&lock2);
        std::cout << "消费者线程："<<name.c_str()<<"放入分数：" << data << std::endl;
        pthread_mutex_unlock(&lock2);
        sleep(1);
    }
}
////消费者和生产者  是既是同步  也是互斥！  同步是两个线程运行  互斥是加减互斥
// 生产者
void *proutine(void *argc)
{
    int num = Getnameid();
    std::string name = "thread-" + std::to_string(num);
    blackqueue<int> *p = static_cast<blackqueue<int> *>(argc);
    while (true)
    {
        int data = pushscore();
        p->push_back(data);
        pthread_mutex_lock(&lock2);
        std::cout << "生产者线程"<<name.c_str()<<"：放入分数" << data << std::endl;
        pthread_mutex_unlock(&lock2);
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    blackqueue<int> *pq = new blackqueue<int>;
    pthread_t c[3], p[3];
    pthread_create(&c[0], nullptr, croutine, (void *)pq);
    pthread_create(&c[0], nullptr, croutine, (void *)pq);
    pthread_create(&c[1], nullptr, croutine, (void *)pq);
    pthread_create(&p[1], nullptr, proutine, (void *)pq);
    pthread_create(&p[2], nullptr, proutine, (void *)pq);
    pthread_create(&p[2], nullptr, proutine, (void *)pq);
    while (1)
    {
    }
    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(c[2], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);
    return 0;
}
