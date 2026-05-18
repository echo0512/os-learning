用 fork + execvp 调用 clang 编译 C 文件

## 1. 练习目标

本练习的目标是：不用 `system()` 或 `popen()`，而是通过 `fork + execvp + waitpid` 在 C 程序中调用编译器。即希望程序自动完成类似下面这条命令的事情：

```bash
clang /tmp/hello.c -o /tmp/hello
```

---

## 2. 本练习的程序设计

本练习分成两个文件：

```text
hello.c       被编译的 C 源文件
main.c       调用 clang 的控制程序
```

其中 `hello.c` 是一个普通 C 程序，例如：

```c
#include <stdio.h>

int main() {
    printf("hello from compiled program\n");
    return 0;
}
```

`main.c` 的任务不是直接执行 `hello.c`，而是调用 clang 编译它：

```text
main
    ↓
fork
    ↓
子进程 execvp("clang", ...)
    ↓
clang 编译 hello.c
    ↓
生成 hello
    ↓
父进程 waitpid 等待 clang 结束
    ↓
父进程判断编译是否成功
```

---

## 5. 核心流程图

```text
父进程启动 main
        ↓
调用 fork()
        ↓
分裂出父进程和子进程

子进程：
        ↓
准备 clang 参数数组
        ↓
execvp("clang", argv)
        ↓
子进程被替换成 clang
        ↓
clang 编译 hello.c
        ↓
clang 退出

父进程：
        ↓
waitpid(pid, &status, 0)
        ↓
等待 clang 进程结束
        ↓
通过 status 判断编译结果
        ↓
输出 compile success 或 compile failed
```