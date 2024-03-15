#include <stdio.h>

#ifdef DYNAMIC_DLOPEN
    #include "collatzlib/collatz.c"
    #include "dlfcn.h"
#endif

int main() {
#ifdef DYNAMIC_DLOPEN
    void* dll_handle = dlopen("collatzlib/build/libcollatz_shared.so", RTLD_LAZY);
    if (!dll_handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }
    int (*collatz_conjecture)(int) = (int (*)(int))dlsym(dll_handle, "collatz_conjecture");
    int (*test_collatz_convergence)(int, int) = (int (*)(int, int))dlsym(dll_handle, "test_collatz_convergence");
#endif
    int x = 10;
    int iter = 100;
    printf("Test collatz for %d in max %d iters: %d\n", x, iter, test_collatz_convergence(x, iter));
#ifdef DYNAMIC_DLOPEN
    dlclose(dll_handle);
#endif
}