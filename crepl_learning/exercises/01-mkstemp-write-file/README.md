把一段 C 代码字符串写入 /tmp 下的临时 .c 文件


## 1. 为什么临时文件要放在 `/tmp/`？

不要默认在当前目录创建文件，因为：

```text
1. 当前目录可能没有写权限；
2. 当前目录可能是只读目录；
3. 多个程序同时运行时容易冲突；
4. 临时文件本来就适合放在 /tmp 中。
```

---

## 2. `mkstemps()` 的作用

### 2.1 基本作用

`mkstemps()` 用来安全创建一个唯一的临时文件。

函数形式：

```c
int mkstemps(char *template, int suffixlen);
```

示例：

```c
char path[] = "/tmp/crepl-test-XXXXXX.c";
int fd = mkstemps(path, 2);
```

调用成功后：
- path 会被修改成真实文件名
- fd 是已经打开的文件描述符

例如：

```text
调用前：/tmp/crepl-test-XXXXXX.c
调用后：/tmp/crepl-test-aB91kQ.c
```

---

### 2.2 必须使用可修改的字符数组

正确写法：

```c
char path[] = "/tmp/crepl-test-XXXXXX.c";
```

错误写法：

```c
char *path = "/tmp/crepl-test-XXXXXX.c";
```

原因：
mkstemps() 会修改 path 中的 XXXXXX。
字符串字面量通常不能被修改，所以不能用 char * 指向字符串字面量。

---

### 2.3 模板中必须有 6 个 `X`

正确：

```c
char path[] = "/tmp/crepl-test-XXXXXX.c";
```

错误：

```c
char path[] = "/tmp/crepl-test-XXX.c";
```

`mkstemps()` 需要用 6 个 `X` 生成随机文件名。

---

### 2.4 `suffixlen` 表示后缀长度

如果模板是：

```c
char path[] = "/tmp/crepl-test-XXXXXX.c";
```

后缀是：

```text
.c
```

长度是 2，所以：

```c
mkstemps(path, 2);
```

如果模板是：

```c
char path[] = "/tmp/test-XXXXXX.txt";
```

后缀是：

```text
.txt
```

长度是 4，所以：

```c
mkstemps(path, 4);
```

---

## 3. 文件描述符 `fd` 是什么？

`mkstemps()` 返回的是文件描述符：

```c
int fd = mkstemps(path, 2);
```

文件描述符可以理解为：

```text
操作系统给当前进程的一个编号，用来表示一个已经打开的文件。
```

常见文件描述符：

```text
0：标准输入 stdin
1：标准输出 stdout
2：标准错误 stderr
3、4、5...：程序打开的其他文件
```

如果 `mkstemps()` 成功，返回一个非负整数。

如果失败，返回：

```c
-1
```

所以要检查：

```c
if (fd == -1) {
    perror("mkstemps");
    return 1;
}
```

---

## 4. `write()` 的作用

### 4.1 基本作用

`write()` 用来向文件描述符写入数据。

函数形式：

```c
ssize_t write(int fd, const void *buf, size_t count);
```

示例：

```c
const char *code = "int add(int a, int b) { return a + b; }\n";
ssize_t n = write(fd, code, strlen(code));
```

含义：
把 code 指向的字符串内容写入 fd 对应的文件中。

---

### 4.2 参数解释

```c
write(fd, code, strlen(code));
```

| 参数             | 含义        |
| -------------- | --------- |
| `fd`           | 写入哪个文件    |
| `code`         | 要写入的数据在哪里 |
| `strlen(code)` | 要写入多少字节   |

---

## 5. 程序逻辑

整体流程：

```text
1. 准备临时文件路径模板
2. 调用 mkstemps() 创建唯一临时文件
3. 检查创建是否成功
4. 准备要写入的 C 代码字符串
5. 使用 write() 写入文件
6. 检查写入是否成功
7. close() 关闭文件
8. printf() 打印文件路径
```

