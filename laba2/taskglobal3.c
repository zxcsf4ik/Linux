#include <stdio.h>
#include <stdlib.h>

int global_init = 42;
int global_uninit;

int main() {
    int local;
    int *heap = malloc(4);
    static int static_init = 7;
    static int static_uninit;

    printf("text (main):        %p\n", (void*)main);
    printf("data (global_init): %p\n", (void*)&global_init);
    printf("bss (global_uninit):%p\n", (void*)&global_uninit);
    printf("data (static_init): %p\n", (void*)&static_init);
    printf("bss (static_uninit):%p\n", (void*)&static_uninit);
    printf("heap:               %p\n", (void*)heap);
    printf("stack (local):      %p\n", (void*)&local);

    free(heap);
    return 0;
}
