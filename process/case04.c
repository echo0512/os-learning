//父进程创建多个子进程
//注意细节防止fork炸弹

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    for(int i=0;i<5;i++){
        pid_t pid = fork();
        if(pid < 0){
            perror("fork failed\n");
            exit(1);
        }else if(pid == 0){
            printf("Child %d, pid = %d\n",i,getpid());
            return 0;
        }
    }

    for(int i=0;i<5;i++){
        wait(NULL);
    }
    return 0;
}