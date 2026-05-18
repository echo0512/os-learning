#include <stdio.h>
#include <dlfcn.h>

int main() {
    void *handle = dlopen("./mylib.dylib", RTLD_NOW);

    if (handle == NULL) {
        printf("dlopen failed: %s\n", dlerror());
        return 1;
    }

    int (*add_ptr)(int, int);

    add_ptr = dlsym(handle, "add");

    if (add_ptr == NULL) {
        printf("dlsym failed: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    int result = add_ptr(2, 3);
    printf("add(2, 3) = %d\n", result);

    dlclose(handle);
    return 0;
}