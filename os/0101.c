#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// EBUSY
#include <errno.h>

int counter = 0; /*两个线程都能访问的共享变量 */

void thread1(void*arg);
void thread2(void*arg);

// shared mutex variable
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char*argv[]) {
    pthread_t id1, id2;

    pthread_mutex_t mutex;
    pthread_mutex_init (&mutex, NULL);
    /*创建第1个线程*/
    pthread_create( & id1, NULL, (void*)thread1, NULL);

    /*创建第2个线程*/
    pthread_create( & id2, NULL, (void*)thread2, NULL);

    /*等待第1个线程结束*/
    pthread_join(id1, NULL);
    /*等待第2个线程结束*/
    pthread_join(id2, NULL);


    /*打印counter最后的值*/
    printf("before exit counter=%d \n", counter);

    exit(0);
}

int inc_counter () {

    do {
        if (pthread_mutex_trylock (&counter_mutex) != EBUSY) {
            ++counter;
            pthread_mutex_unlock (&counter_mutex);
            break;
        }
    } while (1);

    return counter;
}

/*第1个线程执行代码*/
void thread1(void*arg) {
    int i, val;
    for (i = 1; i <= 5; i++) {
        val = inc_counter();

        /*LINE A*/
        printf("thread 1 iter%d lineA counter=%d \n", i, counter);

        /*睡眠或挂起*/
        // micro second
        usleep(100000);

        /*LINE B*/
        printf("thread 1 iter%d lineB counter=%d \n", i, counter);

        counter = val;
    }
}

/*第2个线程执行代码*/
void thread2(void*arg) {
    int i, val;
    for (i = 1; i <= 5; i++) {
        val = inc_counter();

        /*睡眠或挂起*/
        usleep(9000);

        printf("thread 2 iter%d       counter=%d \n", i, counter);

        counter = val;
    }
}
