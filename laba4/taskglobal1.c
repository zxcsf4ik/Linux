#include <stdio.h>
#include <stdint.h>
#include <limits.h>

int main() {
    printf("Архітектура: %zu-біт\n", sizeof(void*) * 8);
    printf("Максимальне значення size_t: %zu\n", (size_t)-1);
    return 0;
}
