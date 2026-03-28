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
