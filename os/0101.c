#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int counter = 0; /*两个线程都能访问的共享变量 */

void thread1(void*arg);
void thread2(void*arg);

int main(int argc, char*argv[]) {
    pthread_t id1, id2;

    /*创建第1个线程*/
    pthread_create( & id1, NULL, (void*)thread1, NULL);

    /*创建第2个线程*/
    pthread_create( & id2, NULL, (void*)thread2, NULL);

    /*等待第1个线程结束*/
    pthread_join(id1, NULL);
    /*等待第2个线程结束*/
    pthread_join(id2, NULL);


    /*打印counter最后的值*/
    printf("最后的counter值为%d \n", counter);

    exit(0);
}

/*第1个线程执行代码*/
void thread1(void*arg) {
    int i, val;
    for (i = 1; i <= 5; i++) {
        val = ++counter;

        /*LINE A*/
        printf("第1个线程：第%d次循环，第1次引用counter=%d \n", i, counter);

        /*睡眠或挂起300毫秒钟*/
        usleep(300);

        /*LINE B*/
        printf("第1个线程：第%d次循环，第2次引用counter=%d \n", i, counter);

        counter = val;
    }
}

/*第2个线程执行代码*/
void thread2(void*arg) {
    int i, val;
    for (i = 1; i <= 5; i++) {
        val = ++counter;

        /*睡眠或挂起100毫秒钟*/
        usleep(100);

        printf("第2个线程：第%d次循环，counter=%d \n", i, counter);

        counter = val;
    }
}
