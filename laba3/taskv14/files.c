#include <stdio.h>

int main() {

    FILE *f[100];

    for (int i = 0; i < 100; i++) {

        char name[20];

        sprintf(name, "file%d.txt", i);

        f[i] = fopen(name, "w");

        if (f[i] == NULL) {
            printf("Cannot open file %d\n", i);
            return 1;
        }

        printf("Opened file %d\n", i);
    }

    return 0;
}
