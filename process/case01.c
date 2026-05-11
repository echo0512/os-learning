//进程api  fork()、wait()
//如何创建一个子进程
//父进程是否wait的影响，观察父子进程号
//父子进程变量是否共享
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int cnt = 0;//验证变量是否共享
    pid_t pid = fork();
    if(pid<0){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        // 子进程
        cnt+=1;
        printf("child process, pid = %d, parent pid = %d, cnt is %d\n",getpid(),getppid(),cnt);
    }else{
        // 父进程
        wait(NULL);//注释or不注释掉这行代码看输出差别
        cnt+=2;
        printf("parent process, pid = %d, child pid = %d, cnt is %d\n",getpid(),pid,cnt);
    }
    return 0;
}

//变量不共享，子进程拥有一份完全一样的拷贝，但是在后续更改的时候父子进程的变量不会相互影响