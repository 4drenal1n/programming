Практика 3: Циклы и массивы

**Студент:** Смирнов Артём Валерьевич
**Группа:** ИВТ-123
**Дата:** 15 января 2026 г.

Задание 3.1: Таблица синусов *
Напишите программу, которая выводит таблицу значений sin(x) от 0 до 2π с шагом 0.1.
Формат вывода:
x = 0.00, sin(x) = 0
x = 0.10, sin(x) = 0.0998
x = 0.20, sin(x) = 0.1987

### Код программы

    #include <stdio.h>
    #include <math.h>
    
    int main(void) {
        double pi = 3.14159;
        double end = 2 * pi;

    for (double x = 0; x <= end + 0.0001; x += 0.1) {
        printf("x = %.2lf, sin(x) = %.4lf\n", x, sin(x));
    }

    return 0;
    }

<img width="973" height="507" alt="Снимок экрана 2026-03-28 140018" src="https://github.com/user-attachments/assets/3fddefd5-476c-4553-83e2-46ca4850aac6" />


Задание 3.2: Сумма ряда **
Вычислите сумму гармонического ряда

### Код программы

    #include <stdio.h>
    
    int main() {
        int n;
        double sum = 0.0;

    printf("Enter n: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("n must be positive.\n");
        return 1;
    }

    printf("S = ");
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / i;
        if (i == 1)
            printf("1");
        else
            printf(" + 1/%d", i);
    }
    printf(" = %.4f\n", sum);

    return 0;
    }

<img width="977" height="507" alt="Снимок экрана 2026-03-28 140434" src="https://github.com/user-attachments/assets/c0a52919-479d-4abf-894b-0cfb83e5b926" />



Задание 3.3: Числа Фибоначчи **
Выведите первые N чисел Фибоначчи.
Правило: F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)

### Код программы

    #include <stdio.h>
    
    int main() {
        int n;

    printf("Enter N: ");
    scanf("%d", &n);

    if (n <= 0) {
        
        return 0;
    }

    long long a = 0, b = 1, next;
    printf("Fibonacci:");
    for (int i = 0; i < n; i++) {
        printf(" %lld", a);
        next = a + b;
        a = b;
        b = next;
    }
    printf("\n");

    return 0;
    }

<img width="982" height="507" alt="Снимок экрана 2026-03-28 140956" src="https://github.com/user-attachments/assets/adff2a75-e825-43d6-b726-8710c8c16b9f" />


Задание 3.4: Среднее арифметическое *
Напишите программу, которая:
1 Запрашивает 5 чисел с клавиатуры
2 Сохраняет их в массив
3 Вычисляет и выводит среднее арифметическое

### Код программы

    #include <stdio.h>
    
    int main() {
        double numbers[5];
        double sum = 0.0;

    printf("Enter 5 numbers: ");
    for (int i = 0; i < 5; i++) {
        scanf("%lf", &numbers[i]);
        sum += numbers[i];
    }

    double average = sum / 5.0;
    printf("Average: %.2lf\n", average);

    return 0;
    }

<img width="984" height="517" alt="Снимок экрана 2026-03-28 141411" src="https://github.com/user-attachments/assets/28c5aa11-b738-4ed5-86fd-fc056c1a5617" />


Задание 3.5: Минимум и максимум **
Напишите программу, которая находит минимальный и максимальный элементы массива из 10
чисел, введённых с клавиатуры.

### Код программы

    #include <stdio.h>
    
    int main() {
        int arr[10];

    printf("Enter 10 numbers: ");
    for (int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }

    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < 10; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    printf("Minimum: %d\n", min);
    printf("Maximum: %d\n", max);

    return 0;
    }

<img width="984" height="513" alt="Снимок экрана 2026-03-28 141747" src="https://github.com/user-attachments/assets/e3e1a60a-5172-492b-8bc7-c0b223857e35" />


Задание 3.6: Реверс массива **
Напишите программу, которая переворачивает массив “на месте” (без создания нового массива).
Подсказка: Меняйте местами элементы с начала и конца, двигаясь к середине.

### Код программы

    #include <stdio.h>
    
    int main() {
    int arr[5];

    printf("Enter 5 numbers: ");
    for (int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }

    
    printf("Original: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

   
    for (int i = 0; i < 5 / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[5 - 1 - i];
        arr[5 - 1 - i] = temp;
    }

    
    printf("Reversed: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
    }



<img width="981" height="512" alt="Снимок экрана 2026-03-28 142149" src="https://github.com/user-attachments/assets/d9e0c490-3622-4f5a-9bc0-596f8c1783e8" />



