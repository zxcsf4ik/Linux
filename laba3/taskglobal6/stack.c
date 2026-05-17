#include <stdio.h>

void recursion(int n) {

    char buffer[1024];

    printf("Depth: %d\n", n);

    recursion(n + 1);
}

int main() {

    recursion(1);

    return 0;
}
