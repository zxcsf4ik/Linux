# Лабораторна робота №3

## Завдання 3.1 — ліміт відкритих файлів

Експерименти з `ulimit -n` у Docker-контейнері.

### Команди

```bash
ulimit -n
ulimit -aS | grep "open files"
ulimit -aH | grep "open files"
ulimit -n 3000
ulimit -n 3001
ulimit -n 2000
```

### Результати

- За замовчуванням soft limit = 1024, hard limit = 524288
- Можна підняти до 3000
- 3001 — помилка, бо перевищує hard limit

Перевірка та зміна ліміту:
<img width="588" height="318" alt="image" src="https://github.com/user-attachments/assets/41ea7caf-38cf-45d2-85ad-35c617d80875" />


Встановлення ліміту 2000:
<img width="379" height="116" alt="image" src="https://github.com/user-attachments/assets/f88293be-025e-4ca0-b7c5-52d83178f7ae" />


---

## Завдання 3.2 — perf та CPU time

Встановлення ліміту CPU time та запуск нескінченного циклу.

### Команди

```bash
gcc cpu.c -o cpu
ulimit -t 5
./cpu
```

### Результати

Програма вбивається через 5 секунд — `Killed`.

CPU time limit:
<img width="311" height="100" alt="image" src="https://github.com/user-attachments/assets/6f6135a4-1f92-43d1-8bb6-700b9667f167" />

---

## Завдання 3.3 — кидання кубика з лімітом файлу

Програма імітує кидання шестигранного кубика і записує результати у файл.

### Компіляція та запуск

```bash
gcc dice.c -o dice
ulimit -f 1
./dice
```

### Результати

Програма аварійно завершується з помилкою `File size limit exceeded`.

Перевищення ліміту розміру файлу:
<img width="390" height="97" alt="image" src="https://github.com/user-attachments/assets/5690a4ca-248f-44dd-90b3-252a1ecf6df3" />

---

## Завдання 3.4 — лотерея з лімітом CPU

Програма генерує числа для лотереї (7 з 49, 6 з 36).

### Компіляція та запуск

```bash
gcc lottery.c -o lottery
ulimit -t 5
./lottery
```

### Результати

Програма генерує кілька варіантів, потім вбивається через вичерпання CPU time.

Лотерея з лімітом CPU:
<img width="425" height="927" alt="image" src="https://github.com/user-attachments/assets/d7692ec3-d13a-4ad4-adeb-4fb2321ba456" />


---

## Завдання 3.5 — копіювання файлу

Програма копіює один файл в інший з перевіркою аргументів та обробкою ліміту.

### Компіляція та запуск

```bash
gcc copy.c -o copy
ulimit -f 1
./copy big.txt out.txt
```

### Результати

При перевищенні ліміту файлу — `File size limit exceeded (core dumped)`.

Копіювання з перевищенням ліміту:
<img width="543" height="139" alt="image" src="https://github.com/user-attachments/assets/1338ffef-f82b-4d31-a2ef-62c5ae82265b" />

---

## Завдання 3.6 — обмеження стека

Рекурсивна програма для демонстрації ліміту стека.

### Компіляція та запуск

```bash
gcc stack.c -o stack
ulimit -s 128
./stack
```

### Результати

При ліміті 128 KB — досягається глибина ~9, потім segmentation fault.

Стек з лімітом 128 KB:

Стек за замовчуванням (глибина ~112):
<img width="402" height="238" alt="image" src="https://github.com/user-attachments/assets/c10d6f0c-32f7-416a-b337-3546d65bee98" />

<img width="443" height="260" alt="image" src="https://github.com/user-attachments/assets/a37ebb44-9e5b-4b86-a87c-6a3ec2bb33fc" />



---

## Завдання по варіанту (14) — контейнери з різними ulimit

Досліджено роботу механізму ulimit у Docker-контейнерах.

### 1. Обмеження кількості відкритих файлів (nofile)

```bash
docker run -it --ulimit nofile=50:50 ubuntu bash
```

Програма files.c відкриває файли у циклі. Після досягнення ліміту 50 — помилка `Cannot open file`.

Відкриття файлів до досягнення ліміту:
<img width="302" height="889" alt="image" src="https://github.com/user-attachments/assets/e4323de3-9479-4a21-b4c6-04d3d1d8f9fb" />


### 2. Обмеження процесорного часу (cpu)

```bash
docker run -it --ulimit cpu=5 ubuntu bash
```

Програма cpu.c з нескінченним циклом вбивається через 5 секунд.

CPU time limit в обмеженому контейнері:
<img width="294" height="78" alt="image" src="https://github.com/user-attachments/assets/23e1d8d5-0144-4dda-82ab-bc7411348279" />


### 3. Обмеження розміру файлу (fsize)

```bash
docker run -it --ulimit fsize=1000:1000 ubuntu bash
```

При спробі встановити пакети через apt система не могла створювати службові файли. Помилка `Sub-process http received signal 25` — SIGXFSZ (перевищення розміру файлу). Тому дослідження було зроблено ще до тестування на програмі.

Помилка при спробі встановити пакети:
<img width="579" height="83" alt="image" src="https://github.com/user-attachments/assets/9006e1e8-cbd8-4ed5-a9d1-4fd925a084d6" />


### Висновок

- При обмеженні кількості відкритих файлів програма не могла створювати нові файлові дескриптори
- При обмеженні CPU часу процес автоматично завершувався після перевищення ліміту
- При обмеженні розміру файлу навіть системні процеси контейнера не могли нормально працювати
- Docker дозволяє ефективно контролювати використання ресурсів контейнера
