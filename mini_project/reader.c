#include <stdio.h>
#include <stdlib.h>
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

    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    data = mmap(NULL, sizeof(SharedData),
                PROT_READ,
                MAP_SHARED, shm_fd, 0);

    if (data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }

    printf("Temperature: %.2f C\n", data->temperature);
    printf("Voltage: %.2f V\n", data->voltage);
    printf("Device status: %s\n", data->device_status ? "ON" : "OFF");
    printf("Message: %s\n", data->message);

    munmap(data, sizeof(SharedData));
    close(shm_fd);

    shm_unlink(SHM_NAME);

    return 0;
}
