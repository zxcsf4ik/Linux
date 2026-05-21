#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/device_shared_memory"

typedef struct {
    float temperature;
    float voltage;
    int device_status;
    char message[128];
} SharedData;

int main() {
    int shm_fd;
    SharedData *data;

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return 1;
    }

    data = mmap(NULL, sizeof(SharedData),
                PROT_READ | PROT_WRITE,
                MAP_SHARED, shm_fd, 0);

    if (data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }

    data->temperature = 36.6;
    data->voltage = 12.5;
    data->device_status = 1;
    strcpy(data->message, "Device is working normally");

    printf("Data written to shared memory.\n");

    munmap(data, sizeof(SharedData));
    close(shm_fd);

    return 0;
}
