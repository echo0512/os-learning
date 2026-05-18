#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    char path[] = "/tmp/test-XXXXXX.c";
    int fd = mkstemps(path, 2);

    if (fd == -1) {
        perror("mkstemps");
        return 1;
    }

    const char *code = "int add(int a, int b) { return a + b; }\n";

    ssize_t n = write(fd, code, strlen(code));

    if (n == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);

    printf("created file: %s\n", path);
    return 0;
}