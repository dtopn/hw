#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// number of producers
#define P_NUM 3
// number of records put into queue per thread
#define ENQUQUE_LEN 5

// number of consumers
#define C_NUM 5
// number of records put into queue per thread
#define DEQUQUE_LEN 3

// queue length
#define MAX_QUEUE_LEN 8

// maximun length of string buffer
#define MAX_BUFF_LEN 1023

// mutex_lock
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
// queue specific variables, mutexed
int head = 0;
int tail = 0;
int size = 0;
char queue [MAX_QUEUE_LEN][MAX_BUFF_LEN];

void inc(int* posi) {

    if (*posi + 1 == MAX_QUEUE_LEN) {
        *posi = 0;
    }
    else {
        ++*posi;
    }

}

void *send_msg(void* arg) {

    int fake_tid = *(int*) arg;
    int cnt = 0;
    char buff [MAX_BUFF_LEN];

    do {

        // wait for lock
        pthread_mutex_lock (&queue_mutex);

        // if the queue is not full
        if (size != MAX_QUEUE_LEN) {
            // counter for this thread
            ++cnt;
            // create message
            sprintf(buff, "time: %4d iter: %d tid: %d",
                    (int) clock(), cnt, fake_tid);
            strcpy(queue[head], buff);
            // increase head pointer for queue
            inc(&head);
            // update size of the queue
            ++size;
        }
        else {
            printf("Queue full,        tid: %d\n", fake_tid);
        }

        pthread_mutex_unlock (&queue_mutex);

        /* I'd like to take a nap */
        usleep(rand() % 977 + 1);

    } while (cnt < ENQUQUE_LEN);

    return 0;
}

void *read_msg(void* arg) {

    int fake_tid = *(int*) arg;
    int cnt = 0;

    while(cnt < DEQUQUE_LEN) {

        pthread_mutex_lock (&queue_mutex);

        // if the queue is not empty
        if (size > 0) {

            // counter for this thread
            ++cnt;
            // print out result
            printf("%s\n", queue[tail]);
            // increase head pointer for queue
            inc(&tail);
            // update size of the queue
            --size;

        }
        else {
            printf("Queue empty,       tid: %d\n", fake_tid);
        }

        pthread_mutex_unlock (&queue_mutex);

        /* I'd like to take a nap */
        usleep(rand() % 13254 + 1);

    }

    return 0;
}


int fake_tid1 = 1;
int fake_tid2 = 2;
int fake_tid3 = 3;

int fake_tid4 = 4;
int fake_tid5 = 5;
int fake_tid6 = 6;
int fake_tid7 = 7;
int fake_tid8 = 8;

int main(int argc, char **argv) {

    srand (time(NULL));

    // producers
    pthread_t thread1_id;
    pthread_t thread2_id;
    pthread_t thread3_id;
    //consumers
    pthread_t thread4_id;
    pthread_t thread5_id;
    pthread_t thread6_id;
    pthread_t thread7_id;
    pthread_t thread8_id;


    // producers threads
    pthread_create (&thread1_id, NULL, &send_msg, (void*)&fake_tid1);
    pthread_create (&thread2_id, NULL, &send_msg, (void*)&fake_tid2);
    pthread_create (&thread3_id, NULL, &send_msg, (void*)&fake_tid3);
    // consumers threads
    pthread_create (&thread4_id, NULL, &read_msg, (void*)&fake_tid4);
    pthread_create (&thread5_id, NULL, &read_msg, (void*)&fake_tid5);
    pthread_create (&thread6_id, NULL, &read_msg, (void*)&fake_tid6);
    pthread_create (&thread7_id, NULL, &read_msg, (void*)&fake_tid7);
    pthread_create (&thread8_id, NULL, &read_msg, (void*)&fake_tid8);

    // waiting all threads to finish
    pthread_join (thread1_id, NULL);
    pthread_join (thread2_id, NULL);
    pthread_join (thread3_id, NULL);

    pthread_join (thread4_id, NULL);
    pthread_join (thread5_id, NULL);
    pthread_join (thread6_id, NULL);
    pthread_join (thread7_id, NULL);
    pthread_join (thread8_id, NULL);

    return 0;
}
