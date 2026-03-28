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
