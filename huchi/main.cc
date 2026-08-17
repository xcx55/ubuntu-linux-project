#include "pthread.cc"
int ticket = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Getticket()
{
    while (1)
    {
        ticket--;
        printf("线程%d正在抢票!\n", ticket);
        sleep(1);
    }
}

void pushticket()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        ticket++;
        printf("线程%d正在补票!\n", ticket);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    threadmodule::thread a(Getticket);
    threadmodule::thread b(Getticket);
    threadmodule::thread c(Getticket);
    threadmodule::thread d(pushticket);
    threadmodule::thread e(pushticket);
    threadmodule::thread f(pushticket);
    a.start();
    b.start();
    c.start();
    d.start();
    e.start();
    f.start();
    a.join();
    b.join();
    c.join();
    d.join();
    e.join();
    f.join();

    return 0;
}