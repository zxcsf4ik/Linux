#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");

    if (src == NULL) {
        printf("Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    FILE *dst = fopen(argv[2], "wb");

    if (dst == NULL) {
        printf("Cannot open file %s for writing\n", argv[2]);
        fclose(src);
        return 1;
    }

    char buffer[1024];

    while (1) {

        size_t bytesRead = fread(buffer, 1, sizeof(buffer), src);

        if (bytesRead == 0) {
            break;
        }

        size_t bytesWritten = fwrite(buffer, 1, bytesRead, dst);

        if (bytesWritten < bytesRead) {
            printf("File too large\n");

            fclose(src);
            fclose(dst);

            return 1;
        }
    }

    fclose(src);
    fclose(dst);

    printf("File copied successfully\n");

    return 0;
}
