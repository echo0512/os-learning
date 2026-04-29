//条件变量 生产者-消费者问题
//pthread_cond_t
//pthread_cond_wait 
//pthread_cond_signal
//pthread_ocond_broadcast

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;//缓冲区中数据个数

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void* producer(void* arg){
    int item = 0;

    while(1){
        //attention 1
        pthread_mutex_lock(&mutex);

        //attention 2
        while(count == BUFFER_SIZE){
            printf("Buffer full,producer waiting.\n");
            pthread_cond_wait(&cond_producer,&mutex);
        }

        buffer[count++] = item;
        printf("Produced:%d, buffer count:%d\n",item,count);
        item++;
    
        //attention 3
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);

        while(count == 0){
            printf("Buffer empty,consumer waiting.\n");
            pthread_cond_wait(&cond_consumer,&mutex);
        }

        int item = buffer[--count];
        printf("Consumed: %d, buffer count: %d\n", item, count);

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }
    return NULL;
}

int main(){
    pthread_t prod,cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}