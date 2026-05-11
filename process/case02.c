//execlp()
//exec()家族函数 l代表参数以list形式写在函数中，末尾用NULL结束 p表示到环境变量中去找
//常用于参数固定的时候使用，一般使用较少

#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if(pid < 0){
        perror("fork failed");
        exit(1);
    }else if(pid == 0){
        printf("child process begin, pid = %d\n", getpid());
        execlp("ls", "ls", "-l", "-a", NULL);
        //execlp("history","history",NULL);假如是这一行就不会跳转，因为history是shell内置命令
        printf("execlp: No such file or directory\n");
    }else{
        printf("parent process begin, pid = %d\n", getpid());
        wait(NULL);
        printf("child process end\n");
    }
    return 0;
}