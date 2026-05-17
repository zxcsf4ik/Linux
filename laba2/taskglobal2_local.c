#include <stdio.h>

void f() {
    int big[10000];
    int small[100] = {1, 2, 3};
    big[0] = small[0] + small[1] + small[2];
    printf("local arrays ok, big[0]=%d\n", big[0]);
}

int main() {
    f();
    return 0;
}
