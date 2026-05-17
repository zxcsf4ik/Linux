#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE (50 * 1024 * 1024)

void print_memory() {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "grep -E '^(VmSize|VmRSS)' /proc/%d/status", getpid());
    system(cmd);
}

int main() {
    printf("=== Copy-On-Write Demo ===\n\n");
    
    printf("До алокації:\n");
    print_memory();
    
    char *buf = malloc(SIZE);
    printf("\nПосле malloc:\n");
    print_memory();
    
    memset(buf, 'X', SIZE);
    printf("\nПосле запису:\n");
    print_memory();
    
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("\n[CHILD] Memory:\n");
        print_memory();
        
        memset(buf, 'Y', SIZE / 2);
        printf("[CHILD] After modification:\n");
        print_memory();
        
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        printf("\n[PARENT] Parent data: %c\n", buf[0]);
        printf("[PARENT] Memory:\n");
        print_memory();
    }
    
    free(buf);
    return 0;
}

