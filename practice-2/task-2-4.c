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
