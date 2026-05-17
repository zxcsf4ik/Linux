#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    while (1) {

        printf("7 from 49: ");

        for (int i = 0; i < 7; i++) {
            printf("%d ", rand() % 49 + 1);
        }

        printf("\n");

        printf("6 from 36: ");

        for (int i = 0; i < 6; i++) {
            printf("%d ", rand() % 36 + 1);
        }

        printf("\n\n");
    }

    return 0;
}
