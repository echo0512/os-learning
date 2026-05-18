//前3个实验汇总

// 1. 创建临时 .c 文件
// 2. 写入 add 函数代码
// 3. fork + execvp 调用 clang
// 4. 把 .c 编译成 .dylib
// 5. dlopen 加载 .dylib
// 6. dlsym 找到 add
// 7. 调用 add(10, 20)
// 8. 输出 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

int compile_dylib(const char *c_path, const char *dylib_path) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        char *argv[] = {
            "clang",
            "-dynamiclib",
            "-fPIC",
            (char *)c_path,
            "-o",
            (char *)dylib_path,
            NULL
        };

        execvp("clang", argv);

        perror("execvp clang failed");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return -1;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        return 0;
    }

    printf("clang failed\n");
    return -1;
}

int main(){
    //得到临时c文件的路径
    char c_path[] = "/tmp/crepl-func-XXXXXX.c";

    int fd = mkstemps(c_path,2);

    if(fd==-1){
        perror("mkstemps c_path failed");
        return 1;
    }

    //写入临时c文件内容
    const char *code = 
        "int add(int a, int b) {\n"
        "    return a + b;\n"
        "}\n";
    ssize_t n = write(fd,code,strlen(code));

    if(n == -1){
        perror("write failed");
        close(fd);
        return 1;
    }

    close(fd);

    printf("created c file:%s successfully\n",c_path);

    //生成对应.dylib路径
    char dylib_path[] = "/tmp/crepl-func-XXXXXX.dylib";

    int dylib_fd = mkstemps(dylib_path,6);

    if(dylib_fd==-1){
        perror("mkstemps dylib_path failed");
        return 1;
    }

    //调用clang生成.dylib文件
    if (compile_dylib(c_path, dylib_path) != 0) {
        printf("compile failed\n");
        return 1;
    }

    printf("compile success\n");

    //动态加载、使用库
    void *handle = dlopen(dylib_path, RTLD_NOW);

    if (handle == NULL) {
        printf("dlopen failed: %s\n", dlerror());
        return 1;
    }

    printf("dlopen success\n");

    int (*add_ptr)(int, int);

    add_ptr = dlsym(handle, "add");

    if (add_ptr == NULL) {
        printf("dlsym failed: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    printf("dlsym success\n");

    int result = add_ptr(10, 20);
    printf("add(10, 20) = %d\n", result);

    dlclose(handle);
    return 0;
}
