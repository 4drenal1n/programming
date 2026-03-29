Практическая работа 5 Функции и массивы (продвинутый уровень)


**Студент:** Смирнов Артём Валерьевич
**Группа:** ИВТ-123
**Дата:** 15 января 2026 г.


Задание 5.1: Статистика массива
Напишите функцию, которая вычисляет статистику массива чисел с плавающей точкой.

### Код программы

    #include <stdio.h>
    #include <math.h>

    typedef struct {
        double min;
        double max;
        double sum;
        double average;
        double variance; // дисперсия
    } ArrayStats;

    ArrayStats calculate_stats(const double *arr, size_t size) {
        ArrayStats stats = {0.0, 0.0, 0.0, 0.0, 0.0};

    if (size == 0 || arr == NULL) {
        return stats; // пустой массив – все поля нулевые
    }

    stats.min = stats.max = arr[0];
    stats.sum = arr[0];

    for (size_t i = 1; i < size; ++i) {
        if (arr[i] < stats.min) stats.min = arr[i];
        if (arr[i] > stats.max) stats.max = arr[i];
        stats.sum += arr[i];
    }

    stats.average = stats.sum / size;

    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < size; ++i) {
        double diff = arr[i] - stats.average;
        sum_sq_diff += diff * diff;
    }
    stats.variance = sum_sq_diff / size;

    return stats;
    }

    int main() {
        double arr[] = {2.5, 4.0, 1.5, 3.0, 5.0};
        size_t n = sizeof(arr) / sizeof(arr[0]);

    ArrayStats s = calculate_stats(arr, n);

    printf("Min: %.2f, Max: %.2f\n", s.min, s.max);
    printf("Sum: %.2f, Average: %.2f\n", s.sum, s.average);
    printf("Variance: %.2f\n", s.variance);

    return 0;
    }
<img width="977" height="515" alt="Снимок экрана 2026-03-29 002609" src="https://github.com/user-attachments/assets/4b1a25b3-f16d-4e78-83bd-d1eb7e002103" />

Задание 5.2: Операции с матрицами (VLA)
Реализуйте набор функций для работы с матрицами произвольного размера.

### Код программы

    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    
    void matrix_random(int rows, int cols, int m[rows][cols], int min, int max) {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                m[i][j] = min + rand() % (max - min + 1);
    }
    
    void matrix_print(int rows, int cols, int m[rows][cols]) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                printf("%4d ", m[i][j]);
            putchar('\n');
        }
    }
    
    void matrix_transpose(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                dst[j][i] = src[i][j];
    }
    
    int matrix_multiply(int r1, int c1, int a[r1][c1],
                        int r2, int c2, int b[r2][c2],
                        int result[r1][c2]) {
        if (c1 != r2) return -1; // несовместимые размеры

    for (int i = 0; i < r1; ++i)
        for (int j = 0; j < c2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < c1; ++k)
                result[i][j] += a[i][k] * b[k][j];
        }
    return 0;
    }
    
    int matrix_trace(int n, int m[n][n]) {
        int trace = 0;
        for (int i = 0; i < n; ++i)
            trace += m[i][i];
        return trace;
    }
    
    int main() {
        srand(time(NULL));

    int a[2][3], b[3][2], c[2][2];

    matrix_random(2, 3, a, 1, 6);
    matrix_random(3, 2, b, 7, 12);

    printf("Matrix A (2x3):\n");
    matrix_print(2, 3, a);
    printf("\nMatrix B (3x2):\n");
    matrix_print(3, 2, b);

    if (matrix_multiply(2, 3, a, 3, 2, b, c) == 0) {
        printf("\nA x B (2x2):\n");
        matrix_print(2, 2, c);
    } else {
        printf("Multiplication impossible\n");
    }

    return 0;
    }
<img width="981" height="514" alt="Снимок экрана 2026-03-29 002627" src="https://github.com/user-attachments/assets/c0dc2540-9e0c-41ae-b86a-87d310994564" />

Задание 5.3: Сортировка с компаратором
Реализуйте универсальную функцию сортировки с callback-компаратором.

### Код программы

    #include <stdio.h>
    #include <string.h>
    #include <windows.h>
    
    typedef int (*Comparator)(const void*, const void*);
    
    void bubble_sort(void *base, size_t count, size_t size, Comparator cmp) {
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                char *a = (char*)base + j * size;
                char *b = (char*)base + (j + 1) * size;
                if (cmp(a, b) > 0) {
                    // swap
                    char tmp[size];
                    memcpy(tmp, a, size);
                    memcpy(a, b, size);
                    memcpy(b, tmp, size);
                }
            }
        }
    }

    // Компараторы для int
    int compare_int_asc(const void *a, const void *b) {
        int ia = *(const int*)a;
        int ib = *(const int*)b;
        return ia - ib;
    }
    
    int compare_int_desc(const void *a, const void *b) {
        return compare_int_asc(b, a);
    }
    
    // Компараторы для строк
    int compare_str_asc(const void *a, const void *b) {
        return strcmp(*(const char**)a, *(const char**)b);
    }
    
    int compare_str_length(const void *a, const void *b) {
        const char *sa = *(const char**)a;
        const char *sb = *(const char**)b;
        return (int)(strlen(sa) - strlen(sb));
    }
    
    // Структура Employee
    typedef struct {
        char name[50];
        int age;
        double salary;
    } Employee;
    
    int compare_employee_by_name(const void *a, const void *b) {
        const Employee *ea = a;
        const Employee *eb = b;
    return strcmp(ea->name, eb->name);
    }
    
    int compare_employee_by_age(const void *a, const void *b) {
        const Employee *ea = a;
        const Employee *eb = b;
        return ea->age - eb->age;
    }
    
    int compare_employee_by_salary(const void *a, const void *b) {
        const Employee *ea = a;
        const Employee *eb = b;
        if (ea->salary < eb->salary) return -1;
        if (ea->salary > eb->salary) return 1;
        return 0;
    }
    
    int main() {
        SetConsoleOutputCP(65001); // UTF-8 для вывода
        SetConsoleCP(65001); // UTF-8 для ввода
        // Пример с int
        int arr[] = {5, 2, 8, 1, 9};
        size_t n = sizeof(arr) / sizeof(arr[0]);

    bubble_sort(arr, n, sizeof(int), compare_int_asc);
    printf("По возрастанию: ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    putchar('\n');

    bubble_sort(arr, n, sizeof(int), compare_int_desc);
    printf("По убыванию:   ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    putchar('\n');

    // Пример с Employee
    Employee emps[] = {
        {"Иван", 25, 45000.0},
        {"Мария", 30, 55000.0},
        {"Пётр", 28, 60000.0}
    };
    size_t emp_count = sizeof(emps) / sizeof(emps[0]);

    bubble_sort(emps, emp_count, sizeof(Employee), compare_employee_by_salary);
    printf("\nСотрудники по зарплате:\n");
    for (size_t i = 0; i < emp_count; ++i) {
        printf("%s, %d, %.2f\n", emps[i].name, emps[i].age, emps[i].salary);
    }

    return 0;
    }
<img width="980" height="513" alt="Снимок экрана 2026-03-29 003218" src="https://github.com/user-attachments/assets/a3a6e229-cb66-4a12-8d32-d6409f043b82" />

Задание 5.4: Функциональные операции над массивами
Реализуйте функции высшего порядка для работы с массивами.

### Код программы

    #include <stdio.h>
    #include <stdlib.h>
    
    
    typedef int (*Transform)(int);
    typedef int (*Predicate)(int);
    typedef int (*Reducer)(int, int);
    typedef void (*Action)(int, size_t);
    
    void array_map(int *arr, size_t size, Transform func) {
        for (size_t i = 0; i < size; ++i)
            arr[i] = func(arr[i]);
    }
    
    size_t array_filter(const int *src, size_t size, int *dst, Predicate pred) {
        size_t count = 0;
        for (size_t i = 0; i < size; ++i)
            if (pred(src[i]))
                dst[count++] = src[i];
        return count;
    }
    
    int array_reduce(const int *arr, size_t size, int initial, Reducer func) {
        int result = initial;
        for (size_t i = 0; i < size; ++i)
            result = func(result, arr[i]);
        return result;
    }
    
    void array_foreach(const int *arr, size_t size, Action act) {
        for (size_t i = 0; i < size; ++i)
            act(arr[i], i);
    }

    // Трансформеры
    int square(int x) { return x * x; }
    int negate(int x) { return -x; }
    int double_val(int x) { return x * 2; }
    int abs_val(int x) { return x < 0 ? -x : x; }
    
    // Предикаты
    int is_even(int x) { return x % 2 == 0; }
    int is_positive(int x) { return x > 0; }
    int is_greater_than_10(int x) { return x > 10; }
    
    // Редьюсеры
    int sum(int a, int b) { return a + b; }
    int product(int a, int b) { return a * b; }
    int max(int a, int b) { return a > b ? a : b; }
    int min(int a, int b) { return a < b ? a : b; }
    
    void print_element(int x, size_t idx) {
        printf("arr[%zu] = %d\n", idx, x);
    }

    int main() {
    
    int arr[] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    putchar('\n');

    // Map
    array_map(arr, n, square);
    printf("After map(square): ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    putchar('\n');

    // Filter
    int filtered[10];
    size_t filtered_cnt = array_filter(arr, n, filtered, is_positive);
    printf("After filter(is_positive): ");
    for (size_t i = 0; i < filtered_cnt; ++i) printf("%d ", filtered[i]);
    putchar('\n');

    // Reduce
    int total = array_reduce(filtered, filtered_cnt, 0, sum);
    printf("Reduce(sum, 0): %d\n", total);

    // ForEach
    printf("ForEach:\n");
    array_foreach(filtered, filtered_cnt, print_element);

    return 0;
    }
<img width="977" height="508" alt="Снимок экрана 2026-03-29 004612" src="https://github.com/user-attachments/assets/f9938b11-498b-4566-b75c-de6b86092666" />

Задание 5.5: Поиск в массиве
Реализуйте различные алгоритмы поиска.

### Код программы

    #include <stdio.h>
    #include <stdbool.h>
    #include <windows.h>
    
    // Тип предиката: функция, принимающая int и возвращающая int (0/1)
    typedef int (*Predicate)(int);
    
    // Линейный поиск
    int linear_search(const int *arr, size_t size, int target) {
        for (size_t i = 0; i < size; ++i) {
            if (arr[i] == target) return (int)i;
        }
        return -1;
    }
    
    // Бинарный поиск (массив должен быть отсортирован по возрастанию)
    int binary_search(const int *arr, size_t size, int target) {
        int left = 0, right = (int)size - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target) return mid;
            else if (arr[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }
    
    // Поиск первого элемента, удовлетворяющего предикату
    int find_if(const int *arr, size_t size, Predicate pred) {
        for (size_t i = 0; i < size; ++i) {
            if (pred(arr[i])) return (int)i;
        }
        return -1;
    }
    
    // Подсчёт элементов, удовлетворяющих предикату
    size_t count_if(const int *arr, size_t size, Predicate pred) {
        size_t count = 0;
        for (size_t i = 0; i < size; ++i) {
            if (pred(arr[i])) ++count;
        }
        return count;
    }
    
    // Проверка: все ли элементы удовлетворяют предикату
    int all_of(const int *arr, size_t size, Predicate pred) {
        for (size_t i = 0; i < size; ++i) {
            if (!pred(arr[i])) return 0;
        }
        return 1;
    }

    // Проверка: есть ли хотя бы один элемент, удовлетворяющий предикату
    int any_of(const int *arr, size_t size, Predicate pred) {
        for (size_t i = 0; i < size; ++i) {
            if (pred(arr[i])) return 1;
        }
        return 0;
    }

    // Проверка: ни один элемент не удовлетворяет предикату
    int none_of(const int *arr, size_t size, Predicate pred) {
    for (size_t i = 0; i < size; ++i) {
        if (pred(arr[i])) return 0;
    }
    return 1;
    }

    // Примеры предикатов
    int is_even(int x) { return x % 2 == 0; }
    int is_odd(int x) { return x % 2 != 0; }
    int is_greater_than_10(int x) { return x > 10; }
    int is_negative(int x) { return x < 0; }
    int is_positive(int x) { return x > 0; }

    int main() {

    SetConsoleOutputCP(65001); // UTF-8 для вывода
    SetConsoleCP(65001);
    int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Массив: ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    printf("\n\n");

    int target1 = 10;
    int idx = linear_search(arr, n, target1);
    printf("linear_search(%d): %s\n", target1, idx != -1 ? "найден" : "не найден");
    if (idx != -1) printf("  позиция %d\n", idx);

    int target2 = 14;
    idx = binary_search(arr, n, target2);
    printf("binary_search(%d): %s\n", target2, idx != -1 ? "найден" : "не найден");
    if (idx != -1) printf("  позиция %d\n", idx);

    int first_gt10 = find_if(arr, n, is_greater_than_10);
    printf("find_if(is_greater_than_10): первый элемент >10 на позиции %d (значение %d)\n",
           first_gt10, arr[first_gt10]);

    size_t cnt = count_if(arr, n, is_greater_than_10);
    printf("count_if(is_greater_than_10): %zu\n", cnt);

    printf("all_of(is_even): %s\n", all_of(arr, n, is_even) ? "true" : "false");
    printf("any_of(is_negative): %s\n", any_of(arr, n, is_negative) ? "true" : "false");
    printf("none_of(is_negative): %s\n", none_of(arr, n, is_negative) ? "true" : "false");

    return 0;
    }
<img width="982" height="511" alt="Снимок экрана 2026-03-29 144407" src="https://github.com/user-attachments/assets/8e9b4891-9ade-4f41-85de-e6e23f4cca4e" />

Задание 5.6: Работа с двумерными массивами
Реализуйте функции для обработки изображений (матрица яркости пикселей).

### Код программы

    #include <stdio.h>
    #include <windows.h>

    #define MAX_BRIGHTNESS 255

    // Инвертирование (негатив)
    void image_invert(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            img[i][j] = MAX_BRIGHTNESS - img[i][j];
    }

    // Увеличение яркости
    void image_brighten(int rows, int cols, int img[rows][cols], int delta) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int val = img[i][j] + delta;
            if (val < 0) val = 0;
            if (val > MAX_BRIGHTNESS) val = MAX_BRIGHTNESS;
            img[i][j] = val;
        }
      }
    }

    // Пороговая фильтрация (бинаризация)
    void image_threshold(int rows, int cols, int img[rows][cols], int threshold) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            img[i][j] = (img[i][j] > threshold) ? MAX_BRIGHTNESS : 0;
    }
  
    // Размытие (усреднение с соседями)
    void image_blur(int rows, int cols, int src[rows][cols], int dst[rows][cols]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int sum = 0;
            int count = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        sum += src[ni][nj];
                        ++count;
                    }
                }
            }
            dst[i][j] = sum / count;
        }
    }
    }

    // Поворот на 90° по часовой стрелке
    void image_rotate_90(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            dst[j][rows - 1 - i] = src[i][j];
    }

    // Отражение по горизонтали
    void image_flip_horizontal(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols / 2; ++j) {
            int tmp = img[i][j];
            img[i][j] = img[i][cols - 1 - j];
            img[i][cols - 1 - j] = tmp;
        }
    }
    }

    // Отражение по вертикали
    void image_flip_vertical(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows / 2; ++i) {
        for (int j = 0; j < cols; ++j) {
            int tmp = img[i][j];
            img[i][j] = img[rows - 1 - i][j];
            img[rows - 1 - i][j] = tmp;
        }
    }
    }

    // Вспомогательная функция для вывода изображения
    void print_image(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            printf("%4d ", img[i][j]);
        putchar('\n');
    }
    }

    int main() {
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);

    // Исходное изображение 4x4
    int img[4][4] = {
        {100, 150, 200, 250},
        {50,  100, 150, 200},
        {0,   50,  100, 150},
        {25,  75,  125, 175}
    };

    printf("Исходное изображение:\n");
    print_image(4, 4, img);

    // Инвертирование
    image_invert(4, 4, img);
    printf("\nПосле invert:\n");
    print_image(4, 4, img);

    // Восстановим исходное для следующей операции
    int original[4][4] = {
        {100, 150, 200, 250},
        {50,  100, 150, 200},
        {0,   50,  100, 150},
        {25,  75,  125, 175}
    };
    // Пороговая фильтрация
    image_threshold(4, 4, original, 100);
    printf("\nПосле threshold(100):\n");
    print_image(4, 4, original);

    // Демонстрация других функций (на копии)
    int src[4][4] = {
        {100, 150, 200, 250},
        {50,  100, 150, 200},
        {0,   50,  100, 150},
        {25,  75,  125, 175}
    };
    int dst[4][4];
    image_blur(4, 4, src, dst);
    printf("\nРазмытие (blur):\n");
    print_image(4, 4, dst);

    int rotated[4][4];  // после поворота будет 4x4 (исходная была 4x4)
    image_rotate_90(4, 4, src, rotated);
    printf("\nПоворот на 90°:\n");
    print_image(4, 4, rotated);

    int flip_h[4][4];
    // скопируем исходное
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            flip_h[i][j] = src[i][j];
    image_flip_horizontal(4, 4, flip_h);
    printf("\nОтражение по горизонтали:\n");
    print_image(4, 4, flip_h);

    int flip_v[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            flip_v[i][j] = src[i][j];
    image_flip_vertical(4, 4, flip_v);
    printf("\nОтражение по вертикали:\n");
    print_image(4, 4, flip_v);

    return 0;
    }
<img width="981" height="708" alt="Снимок экрана 2026-03-29 144949" src="https://github.com/user-attachments/assets/73d1b483-b492-49ef-8faa-9bde1d77e1f2" />


###Контрольные вопросы
1 Почему sizeof(arr) внутри функции возвращает размер указателя?

В языке C при передаче массива в функцию он «вырождается» (decay) в указатель на свой первый элемент. Параметр, объявленный как int arr[], на самом деле является int *arr. Поэтому внутри функции sizeof(arr) вычисляет размер указателя (обычно 4 или 8 байт в зависимости от архитектуры), а не размер всего массива. Чтобы узнать фактический размер массива, его нужно передавать отдельным параметром (например, size_t size).

2 В чём преимущества и недостатки VLA?

Преимущества:

Позволяют создавать массивы с размером, определяемым во время выполнения (например, int arr[n]).

Упрощают работу с матрицами переменного размера в функциях (параметры-размеры идут перед массивом).

Недостатки:

Размер ограничен размером стека (обычно несколько мегабайт). Большие VLA могут вызвать переполнение стека.

Нельзя изменять размер после создания.

3 Как объявить указатель на функцию, возвращающую указатель на int?

int *(*func_ptr)(int, double);

Это указатель func_ptr на функцию, которая принимает int и double, а возвращает int* (указатель на int).
Общий синтаксис: тип_результата (*имя)(параметры). Здесь результат int*, поэтому * относится к возвращаемому типу.

4 Почему компаратор для qsort принимает const void*?

Это сделано для универсальности и безопасности:

void* позволяет работать с любым типом данных. Функция qsort не знает заранее тип элементов массива.

const гарантирует, что компаратор не будет изменять сравниваемые элементы (он только читает их). Это важно для корректной сортировки и для работы с константными данными.

Приведение типов внутри компаратора выполняется явно: const int *a = (const int*)arg;

5 Как реализовать универсальную функцию swap для любого типа?
Используя void* и размер типа. Пример:

    void universal_swap(void *a, void *b, size_t size) {
        char *pa = (char*)a;
        char *pb = (char*)b;
        char tmp;
        for (size_t i = 0; i < size; ++i) {
            tmp = pa[i];
            pa[i] = pb[i];
            pb[i] = tmp;
        }
    }

