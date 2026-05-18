# Практична робота 4: Динамічне виділення пам'яті

## Завдання 4.1 — Максимальний розмір malloc()

Скільки пам'яті може виділити `malloc(3)` за один виклик?

### Команди

```bash
gcc taskglobal1.c -o taskglobal1_64
gcc -m32 taskglobal1.c -o taskglobal1_32
./taskglobal1_64
./taskglobal1_32
```

### Результати

На 64-бітній системі SIZE_MAX = 18446744073709551615 (2^64-1).
На 32-бітній системі SIZE_MAX = 4294967295 (2^32-1).

Теоретично максимум 2^64, але практичний ліміт 8 EB через розділення User/Kernel адресного простору.

Архітектура і максимальні значення size_t:

<img width="452" height="160" alt="image" src="https://github.com/user-attachments/assets/6c35f7e0-6ec2-402e-8508-fd4766553bab" />


---

## Завдання 4.2 — Переповнення при множенні

Що станеться, якщо передати `malloc(3)` від'ємний аргумент або результат множення переповниться?

### Команди

```bash
gcc taskglobal2.c -o taskglobal2
./taskglobal2
gcc -m32 taskglobal2.c -o taskglobal2_32
./taskglobal2_32
```

### Результати

При множенні 1000000 * 1000000 відбувається переповнення int, результат = -727379968.
При приведенні до size_t отримуємо величезне число.
malloc() не може виділити такий обсяг і повертає NULL.

Результат переповнення і спроба malloc():

<img width="325" height="208" alt="image" src="https://github.com/user-attachments/assets/477c9031-55db-4992-81bb-fb93b1bc347f" />


---

## Завдання 4.3 — malloc(0)

Що станеться, якщо використати `malloc(0)`?

### Команди

```bash
gcc taskglobal3.c -o taskglobal3
ltrace ./taskglobal3
```

### Результати

malloc(0) повертає ненульовий вказівник на мінімальний блок пам'яті.
Цей вказівник є абсолютно валідним і безпечно передавати його у free().

Виклик malloc(0) через ltrace:

<img width="894" height="145" alt="image" src="https://github.com/user-attachments/assets/9005f633-33bc-45e6-b6da-2d9018e37679" />


---

## Завдання 4.4 — Помилка у коді з циклом

Чи є помилки у такому коді?

```c
void *ptr = NULL;
while (<some-condition-is-true>) {
    if (!ptr)
        ptr = malloc(n);
    [... <використання 'ptr'> ...]
    free(ptr);
}
```

### Команди

```bash
gcc taskglobal4.c -o taskglobal4
./taskglobal4
```

### Результати

Програма вбивається з помилкою `double free detected in tcache 2`.

Проблема: після `free(ptr)` вказівник не занулюється. На другій ітерації `if (!ptr)` є false, тому код намагається звільнити вже звільнену пам'ять.

Виправлення: добавити `ptr = NULL;` після `free(ptr);`

Double free помилка при повторній ітерації:

<img width="401" height="136" alt="image" src="https://github.com/user-attachments/assets/824148b9-fb34-4f45-8d37-0908628ee715" />


---

## Завдання 4.5 — Помилка при realloc()

Що станеться, якщо `realloc(3)` не зможе виділити пам'ять?

### Команди

```bash
gcc taskglobal5.c -o taskglobal5
./taskglobal5
```

### Результати

При спробі перевиділити на SIZE_MAX байт realloc() повертає NULL.
Оригінальна пам'ять залишається неушкодженою.

Це критично важливо — потрібно завжди использовать тимчасовий вказівник:

```c
void *tmp = realloc(ptr, new_size);
if (!tmp) {
    free(ptr);
}
```

Помилка при спробі realloc() на SIZE_MAX:

<img width="905" height="238" alt="image" src="https://github.com/user-attachments/assets/344d830f-8376-4e22-848b-e1fb2f084f47" />


---

## Завдання 4.6 — realloc() з NULL або 0

Якщо `realloc(3)` викликати з NULL або розміром 0, що станеться?

### Команди

```bash
gcc taskglobal6.c -o taskglobal6
./taskglobal6
```

### Результати

`realloc(NULL, 100)` повертає вказівник — ведеться як `malloc(100)`.
`realloc(ptr, 0)` повертає NULL — ведеться як `free(ptr)`.

Це рівнозначно специфікації POSIX стандарту.

Поведінка realloc() з NULL та 0:

<img width="312" height="110" alt="image" src="https://github.com/user-attachments/assets/58e0ed4d-ec95-4cab-a3b6-c406020ef1bc" />


---

## Завдання 4.7 — reallocarray()

Перепишіть код, використовуючи `reallocarray(3)` замість `realloc(3)`.

### Команди

```bash
gcc taskglobal7.c -o taskglobal7
ltrace ./taskglobal7
```

### Результати

reallocarray() автоматично множить кількість елементів на їх розмір.
Адреса залишилась тією ж — реалокація сталась на місці.

reallocarray() безпечніша, оскільки перевіряє переповнення при множенні.

Виклик reallocarray() через ltrace:

<img width="693" height="138" alt="image" src="https://github.com/user-attachments/assets/4173b903-3aec-484d-835b-8839e59a9df2" />


---

## Вариант 14 — Copy-On-Write при fork()

Дослідити, як `fork()` впливає на використання пам'яті після великої алокації. Показати ефект Copy-On-Write.

### Команди

```bash
gcc taskv14.c -o taskv14
./taskv14
```

### Результати

**До алокації:** VmSize = 2768 kB, VmRSS = 1884 kB

**После malloc(50MB):** VmSize = 53972 kB, VmRSS = 1888 kB (виртуальна пам'ять виділена, але не в RAM)

**После запису (memset):** VmSize = 53972 kB, VmRSS = 53088 kB (сторінки завантажені в фізичну пам'ять)

**[CHILD] При старті:** Ділить сторінки з батьком через Copy-On-Write механізм

**[CHILD] Після модифікації:** VmRSS = 52272 kB (сторінки скопійовані для дочірнього процесу)

**[PARENT] Після завершення child:** VmRSS = 53152 kB (батьківська пам'ять не змінилась)

Copy-On-Write демонстрація:

<img width="373" height="501" alt="image" src="https://github.com/user-attachments/assets/ac86bfba-378d-422e-bec8-e8a65df565e4" />


---

## Встановлення залежностей

```bash
sudo apt update
sudo apt install -y build-essential ltrace gcc-multilib
```

---

## Висновки

1. **malloc()** максимум обмежена 8 EB на 64-бітних системах через розділення User/Kernel простору
2. **Переповнення int** при множенні призводить до невірних значень при приведенні до size_t
3. **malloc(0)** повертає ненульовий указівник, безпечно передавати у free()
4. **Забування зануління ptr після free()** призводить до double free помилок
5. **realloc()** при помилці повертає NULL, але зберігає оригінальну пам'ять
6. **reallocarray()** переважніша для масивів, оскільки перевіряє переповнення
7. **Copy-On-Write** оптимізує fork(), дозволяючи ділити пам'ять до першого запису
