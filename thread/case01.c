//线程的创建和回收
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//线程函数，接收参数为void*，返回值为void*
void * print_message(void *arg){
    char* msg = (char *)arg;
    printf("%s\n",msg);
    return NULL;
}

int main(){
    pthread_t tid1,tid2;
    
    //创建线程
    pthread_create(&tid1,NULL,print_message,"Hello from thread 1");
    pthread_create(&tid2,NULL,print_message,"Hello from thread 2");

    //回收线程
    pthread_join(tid1,NULL);
    printf("Thread 1 finished\n");
    pthread_join(tid2,NULL);
    printf("Thread 2 finished\n");

    return 0;
}