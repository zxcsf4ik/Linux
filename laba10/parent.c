#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent процес почався (PID: %d)\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        printf("Child процес після fork() (PID: %d)\n", getpid());
        execl("./child", "child", NULL);
        perror("execl");
        exit(1);
    } else {
        printf("Parent чекає на child...\n");
        wait(NULL);
        printf("Child завершився. Parent продовжує.\n");
    }
    
    return 0;
}
