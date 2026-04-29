//互斥锁
//pthread_mutex_t
//pthread_mutex_init
//pthread_mutex_lock
//pthread_mutex_unlock
//pthread_mutex_destroy

#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 10000

int counter_no_lock = 0;
int counter_lock = 0;

pthread_mutex_t mutex;

//不加互斥锁，会有竞态条件，最后输出结果小于50000且每一次执行结果不同
void *increment_no_lock(void *arg){ 
    for(int i = 0; i < NUM_ITERATIONS; i++){
        counter_no_lock++;
    }
    return NULL;
}

void *increment_lock(void *arg){ 
    for(int i = 0; i < NUM_ITERATIONS; i++){
        pthread_mutex_lock(&mutex);
        counter_lock++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_t threads_no_lock[NUM_THREADS];
    pthread_t threads_lock[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads_no_lock[i], NULL, increment_no_lock, NULL);
        pthread_create(&threads_lock[i], NULL, increment_lock, NULL);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads_no_lock[i], NULL);
        pthread_join(threads_lock[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Counter no lock: %d\n", counter_no_lock);
    printf("Counter lock: %d\n", counter_lock);

    return 0;
}