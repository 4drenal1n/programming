#include <stdio.h>

int main() {
    double radius, area;

    printf("Enter radius: ");
    scanf("%lf", &radius);

    area = 3.14159 * radius * radius;

    printf("Area: %.2lf\n", area);

    return 0;
}
