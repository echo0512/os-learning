//mini-shell
//关键命令execvp，适合动态传参数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    char line[1024];

    while(1){
        printf("myshell> ");
        fflush(stdout);
        fgets(line, sizeof(line), stdin);
        line[strcspn(line,"\n")] = 0;

        char *args[100];
        int i=0;
        char *token = strtok(line, " ");

        while(token != NULL){
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        pid_t pid = fork();

        if(pid < 0){
            perror("fork failed");
            exit(1);
        }else if(pid == 0){
            execvp(args[0],args);
            printf("command not found\n");
            return 0;
        }
        wait(NULL);
    }
    return 0;
}
