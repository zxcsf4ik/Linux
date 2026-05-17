#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

int global_init = 10;
int global_uninit;

int main() {
    int local_var;
    int *heap_var = malloc(sizeof(int));

    printf(" time_t researc \n");

    printf("Size of time_t: %zu bytes (%zu bits)\n",
           sizeof(time_t),
           sizeof(time_t) * 8);

    if (sizeof(time_t) == 4) {
        time_t max32 = (time_t)2147483647;

        printf("\n32-bit architecture detected\n");
        printf("Maximum time_t value: %d\n", (int)max32);
        printf("Last valid date: %s", ctime(&max32));

        max32++;

        printf("After overflow: %s", ctime(&max32));
    }
    else if (sizeof(time_t) == 8) {
        time_t max64 = (time_t)9223372036854775807LL;

        printf("\n64-bit architecture detected\n");
        printf("Maximum time_t value: %lld\n", (long long)max64);

        printf("Overflow will occur extremely far in the future\n");
    }

    printf("\n Memory segments \n");

    printf("Text segment  (.text): %p\n", (void*)main);
    printf("Data segment  (.data): %p\n", (void*)&global_init);
    printf("BSS segment   (.bss):  %p\n", (void*)&global_uninit);
    printf("Heap segment  (heap):  %p\n", (void*)heap_var);
    printf("Stack segment (stack): %p\n", (void*)&local_var);

    printf("\n Executable file segments \n");

    system("size taskglobal1");

    free(heap_var);

    return 0;
}
