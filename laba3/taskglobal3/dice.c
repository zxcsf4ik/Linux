#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *f = fopen("dice.txt", "w");

    if (f == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    srand(time(NULL));

    while (1) {
        int dice = rand() % 6 + 1;

        if (fprintf(f, "%d\n", dice) < 0) {
            printf("File size limit exceeded\n");
            fclose(f);
            return 1;
        }

        fflush(f);
    }

    return 0;
}

