#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== Демонстрація фрагментації пам'яті ===\n\n");

    char *a = malloc(1024);
    char *b = malloc(10 * 1024);
    char *c = malloc(1024);

    printf("Етап 1 - Виділили: a(1KB), b(10KB), c(1KB)\n");
    printf("a = %p\n", (void*)a);
    printf("b = %p\n", (void*)b);
    printf("c = %p\n", (void*)c);
    printf("Вільної пам'яті: ~11+ KB\n\n");

    memset(a, 'A', 1024);
    memset(b, 'B', 10 * 1024);
    memset(c, 'C', 1024);

    free(b);
    printf("Етап 2 - Звільнили b (10KB)\n");
    printf("Теоретично вільно: 10 KB + деякі пустоти\n\n");

    printf("Етап 3 - Спробуємо виділити 8KB...\n");
    char *d = malloc(8 * 1024);
    if (d) {
        printf("✓ Успішно: d = %p\n\n", (void*)d);
        free(d);
    } else {
        printf("✗ Невдача: malloc повернув NULL\n\n");
    }

    printf("Етап 4 - Створюємо фрагментацію через чередування алокацій\n");
    
    free(a);
    free(c);
    printf("Звільнили a та c\n");
    printf("Вільна пам'ять: 1KB + 1KB = 2KB роздільно\n\n");

    printf("Етап 5 - Спробуємо виділити 2KB поспіль...\n");
    char *e = malloc(2 * 1024);
    if (e) {
        printf("✓ Успішно (malloc об'єднав вільні блоки)\n");
        free(e);
    } else {
        printf("✗ Невдача через фрагментацію\n");
    }

    printf("\n=== Висновок ===\n");
    printf("Фрагментація = вільна пам'ять розбита на малі шматки\n");
    printf("Результат: велика алокація неможлива, хоч сумарно пам'яти досить\n");

    return 0;
}
