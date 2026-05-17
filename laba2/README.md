# Лабораторна робота №2

## Завдання 2.1 — time_t та сегменти пам'яті

Програма визначає розмір `time_t` і досліджує розташування сегментів у пам'яті.

### Компіляція та запуск

```bash
gcc -O2 -Wall -Wextra -o taskglobal1 taskglobal1.c
./taskglobal1
```

### Результати

На 64-бітній системі `time_t` = 8 байт. Максимальне значення надто велике для виводу дати.
Адреси сегментів йдуть у порядку: text < data/bss < heap < stack.

Запуск taskglobal1 (64-біт):

<img width="732" height="426" alt="image" src="https://github.com/user-attachments/assets/3bd60375-15d0-4d51-b4c9-599ab9328490" />

Запуск taskglobal1 з прапорцем `-m32` (32-біт):

<img width="605" height="383" alt="image" src="https://github.com/user-attachments/assets/354333f1-e0c4-4ce7-a049-e9ccf4f1ce20" />
---

## Завдання 2.2 — сегменти виконуваного файлу

Досліджено вплив різних типів змінних на розмір сегментів.

### Файли

- `taskglobal2_base.c` — базова програма
- `taskglobal2_bss.c` — глобальний масив 1000 int без ініціалізації
- `taskglobal2_data.c` — глобальний масив 1000 int з ініціалізацією
- `taskglobal2_local.c` — локальні масиви у функції

### Компіляція

```bash
gcc -Wall -o taskglobal2_base taskglobal2_base.c && size taskglobal2_base
gcc -Wall -o taskglobal2_bss taskglobal2_bss.c && size taskglobal2_bss
gcc -Wall -o taskglobal2_data taskglobal2_data.c && size taskglobal2_data
gcc -Wall -o taskglobal2_local taskglobal2_local.c && size taskglobal2_local
```

### Результати

- Базова: мінімальні значення всіх сегментів
- BSS: збільшується колонка `bss` на ~4000 байт (1000 × int)
- Data: збільшується колонка `data`, файл на диску теж більший
- Local: сегменти не змінюються, масиви створюються в стеку під час виконання

Результати size для всіх чотирьох програм:
<img width="597" height="240" alt="image" src="https://github.com/user-attachments/assets/689939a6-cc2a-4030-80c2-6f09ddd39da9" />
---

## Завдання 2.3 — розташування сегментів

### Компіляція та запуск

```bash
gcc -Wall -o taskglobal3 taskglobal3.c && ./taskglobal3
```

### Результати

Адреси підтверджують теорію: text — найнижчі, потім data/bss, heap вище, stack — найвищий.

Запуск taskglobal3:

<img width="662" height="167" alt="image" src="https://github.com/user-attachments/assets/a96c95e6-7e54-4ce4-8c9a-6fb26884d018" />
---

## Завдання 2.4 — дослідження стека

### Компіляція та запуск

```bash
gcc -Wall -o taskglobal4 taskglobal4.c
./taskglobal4
```

У другому терміналі:

```bash
ps aux | grep taskglobal4
sudo gdb -p PID -batch -ex "bt"
```

### Результати

Адреси `localvar` йдуть вниз (0x...a4 → 0x...74 → 0x...54 → 0x...34), що підтверджує: стек зростає до менших адрес.

GDB виводить стек викликів: `main` → `foo` → `bar` → `bar_is_now_closed` → `pause`.

Запуск taskglobal4 та GDB backtrace:

<img width="896" height="289" alt="image" src="https://github.com/user-attachments/assets/eb2c60fe-dcd6-4830-b894-95e2f2f6f35d" />

---

## Завдання 2.5 — відповідь

Ні, не можна обійтися без лічильника команд (IP). IP зберігає адресу поточної інструкції, а стек — тільки адреси повернення. Без IP процесор не зможе виконувати інструкції послідовно.

---

## Завдання по варіанту (варіант 14) — аналіз /proc/self/maps

### Компіляція та запуск

```bash
gcc -O2 -Wall -Wextra -o taskv14 taskv14.c
./taskv14
```

### Результати

Програма виводить таблицю всіх сегментів пам'яті поточного процесу: адреси, права доступу, зміщення, шляхи. Загальна кількість сегментів і сумарний обсяг мапленої пам'яті.

Запуск taskv14:

<img width="882" height="586" alt="image" src="https://github.com/user-attachments/assets/f99eab4a-6fca-4ab2-83ea-d7cf2d41ab5c" />

