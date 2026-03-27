Практика 2: Типы данных, арифметика, условные
операторы


**Студент:** Смирнов Артём Валерьевич
**Группа:** ИВТ-123
**Дата:** 15 января 2026 г.

Задание 2.1: Вычисление формулы **
Напишите программу, которая вычисляет значение выражения:
−
√4 ( 2 + 2)3
ℎ( ) = −√
−
3 2 + 2
2 + + + √3 ( − )2
для значений: a = 0.12, b = 3.5, c = 2.4, x = 1.4

### Код программы

#include <stdio.h>
#include <math.h>


int main(void) {
// Шаг 1: Объявляем константы
double a = 0.12, b = 3.5, c = 2.4, x = 1.4;

// Шаг 2: Вычисляем ПЕРВУЮ дробь
double num1 = x - a; // числитель: (x - a)
double denom1 = pow(x*x + a*a, 1.0/3.0); // знаменатель: ∛(x² + a²)
double term1 = num1 / denom1; // первая дробь
// Шаг 3: Вычисляем ВТОРУЮ дробь

double num2 = pow(pow(x*x + b*b, 3), 1.0/4.0); // числитель: ∜((x² + b²)³)
// Или можно записать проще: pow(x*x + b*b, 3.0/4.0)
double denom2 = 2 + a + b + pow(pow(x - c, 2), 1.0/3.0); // знаменатель
double term2 = num2 / denom2; // вторая дробь
// Шаг 4: Собираем результат

double h = -term1 - term2;
printf("h(%.2lf) = %.6lf\n", x, h);
return 0;
}

Задание 2.2: Чётное или нечётное *
Напишите программу, которая определяет, является ли введённое число чётным или нечётным.

### Код программы

#include <stdio.h>

int main() {
    int number;

    printf("Enter number: ");
    scanf("%d", &number);

    if (number % 2 == 0) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }

    return 0;
}

Задание 2.3: Максимум из трёх **
Напишите программу, которая находит максимальное из трёх введённых чисел.

### Код программы

#include <stdio.h>

int main(void) {
    int a, b, c, max;

    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);

    max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }

    printf("Maximum: %d\n", max);

    return 0;
}

Задание 2.4: Тип треугольника **
Напишите программу, которая по трём сторонам определяет тип треугольника:
• Равносторонний — все стороны равны
• Равнобедренный — две стороны равны
• Разносторонний — все стороны разные
• Не существует — если сумма двух сторон <= третьей

### Код программы

#include <stdio.h>

int main(void) {
    int a, b, c;

    printf("Enter sides a, b, c: ");
    scanf("%d %d %d", &a, &b, &c);

    // Проверка существования треугольника
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Triangle does not exist\n");
    }
    else if (a == b && b == c) {
        printf("Equilateral triangle\n");
    }
    else if (a == b || b == c || a == c) {
        printf("Isosceles triangle\n");
    }
    else {
        printf("Scalene triangle\n");
    }

    return 0;
}

Задание 2.5: Квадратное уравнение ***
Напишите программу для решения квадратного уравнения 2 + + = 0:
1 Вычислить дискриминант: = 2√ − 4
2 Если D > 0: два корня
− ±
1,2 = 2
3 Если D = 0: один корень = − 2
4 Если D < 0: корней нет

### Код программы

#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c, discriminant, root1, root2;

    printf("Enter a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        root1 = (-b + sqrt(discriminant)) / (2 * a);
        root2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("D = %.2lf\n", discriminant);
        printf("x1 = %.2lf\n", root1);
        printf("x2 = %.2lf\n", root2);
    }
    else if (fabs(discriminant) < 1e-9) {  // проверка на приблизительное равенство нулю
        root1 = -b / (2 * a);
        printf("D = %.2lf\n", discriminant);
        printf("x = %.2lf\n", root1);
    }
    else {
        printf("D = %.2lf\n", discriminant);
        printf("No real roots\n");
    }

    return 0;
}

Задание 2.6: Калькулятор **
Напишите программу-калькулятор, которая:
1 Запрашивает два числа
2 Запрашивает операцию (+, -, *, /)
3 Выводит результат
Используйте switch для выбора операции.

### Код программы

#include <stdio.h>

int main(void) {
    double num1, num2, result;
    char op;

    printf("Enter first number: ");
    scanf("%lf", &num1);
    printf("Enter second number: ");
    scanf("%lf", &num2);
    printf("Enter operation (+, -, *, /): ");
    scanf(" %c", &op);  // пробел перед %c для пропуска возможного символа новой строки

    switch (op) {
        case '+':
            result = num1 + num2;
            printf("Result: %.2lf + %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '-':
            result = num1 - num2;
            printf("Result: %.2lf - %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '*':
            result = num1 * num2;
            printf("Result: %.2lf * %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero!\n");
            } else {
                result = num1 / num2;
                printf("Result: %.2lf / %.2lf = %.2lf\n", num1, num2, result);
            }
            break;
        default:
            printf("Error: Invalid operation!\n");
    }

    return 0;
}
