#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>

void print_dirty_pages(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/smaps", pid);
    
    FILE *f = fopen(path, "r");
    if (!f) return;
    
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Dirty")) {
            printf("%s", line);
        }
    }
    fclose(f);
}

int main() {
    printf("=== АНАЛІЗ DIRTY PAGES ===\n\n");

    pid_t pid = getpid();
    printf("PID: %d\n\n", pid);

    int fd = open("test_file.txt", O_RDWR);
    
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *shared = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char *priv = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    printf("До модифікації:\n");
    print_dirty_pages(pid);

    printf("\nМодифікуємо SHARED...\n");
    strcpy(shared, "MODIFIED SHARED");
    printf("Після модифікації SHARED:\n");
    print_dirty_pages(pid);

    printf("\nМодифікуємо PRIVATE...\n");
    strcpy(priv, "MODIFIED PRIVATE");
    printf("Після модифікації PRIVATE:\n");
    print_dirty_pages(pid);

    munmap(shared, file_size);
    munmap(priv, file_size);
    close(fd);

    return 0;
}
