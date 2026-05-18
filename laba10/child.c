#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Привіт від child процесу!\n");
    printf("PID: %d\n", getpid());
    return 0;
}
