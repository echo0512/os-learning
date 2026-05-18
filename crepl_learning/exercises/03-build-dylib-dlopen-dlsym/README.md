完成以下这个流程：普通 C 源文件  →  动态库文件

```text
写一个 mylib.c
        ↓
里面定义 add 函数
        ↓
用 clang 把它编译成 mylib.dylib
        ↓
确认这个 .dylib 确实生成成功
```

然后：写另一个程序 load_add.c，在运行时加载 mylib.dylib，找到里面的 add 函数，然后调用它。

```text
load_add 程序启动
    ↓
dlopen 打开 mylib.dylib
    ↓
dlsym 找到 add 的地址
    ↓
把地址转成函数指针
    ↓
调用 add(2, 3)
    ↓
输出结果
```