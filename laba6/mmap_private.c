#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    printf("=== MMAP PRIVATE MODE ===\n\n");

    int fd = open("test_file.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    printf("File size: %ld bytes\n\n", file_size);

    char *addr = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Mapped address: %p\n", (void*)addr);
    printf("Original content:\n%s\n\n", addr);

    printf("Модифікуємо файл (PRIVATE mode)...\n");
    strcpy(addr, "MODIFIED: This is PRIVATE, changes won't affect disk!");
    printf("Modified content:\n%s\n\n", addr);

    printf("Намагаємось синхронізувати...\n");
    if (msync(addr, file_size, MS_SYNC) == -1) {
        printf("msync failed (expected for PRIVATE mode)\n");
        perror("msync");
    }

    printf("\nПеревіримо файл через read()...\n");

    lseek(fd, 0, SEEK_SET);
    char buffer[512];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    printf("Content from disk (unchanged!):\n%s\n\n", buffer);

    munmap(addr, file_size);
    close(fd);

    return 0;
}
