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
