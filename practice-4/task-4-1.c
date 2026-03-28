#include <stdio.h>


long long factorial(int n) {
    if (n < 0) {
        return 0; // факториал отрицательного числа не определён
    }
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n;
    printf("Enter a non-negative integer: ");
    scanf("%d", &n);
    printf("%d! = %lld\n", n, factorial(n));
    return 0;
}
