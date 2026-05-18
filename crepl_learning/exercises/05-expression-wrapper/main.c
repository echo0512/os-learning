#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <expr>\n", argv[0]);
        return 1;
    }

    const char *expr = argv[1];
    int id = 1;

    char code[4096];

    snprintf(code, sizeof(code),
             "int __expr_wrapper_%d() {\n"
             "    return %s;\n"
             "}\n",
             id, expr);

    char c_path[] = "/tmp/crepl-expr-XXXXXX.c";

    int fd = mkstemps(c_path, 2);
    if (fd == -1) {
        perror("mkstemps failed");
        return 1;
    }

    ssize_t n = write(fd, code, strlen(code));
    if (n == -1) {
        perror("write failed");
        close(fd);
        return 1;
    }

    close(fd);

    printf("created wrapper file: %s\n", c_path);
    printf("wrapper function name: __expr_wrapper_%d\n", id);

    return 0;
}