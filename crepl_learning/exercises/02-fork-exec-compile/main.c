#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if(pid<0){
        perror("fork failed\n");
        return 1;
    }

    if(pid == 0){
        char *argv[] = {
            "clang",
            "./hello.c",
            "-o",
            "./hello",
            NULL
        };

        execvp("clang",argv);

        perror("execvp failed");
        _exit(1);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return 1;
    }

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        if (code == 0) {
            printf("compile success\n");
        } else {
            printf("compile failed, exit code = %d\n", code);
        }
    } else {
        printf("child did not exit normally\n");
    }
    
    return 0;
}