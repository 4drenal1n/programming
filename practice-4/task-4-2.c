#include <stdio.h>

// Рекурсивная функция вычисления факториала
long long factorial_rec(int n) {
    if (n < 0) {
        
        return 0;
    }
    if (n == 0) {
        return 1; 
    }
    return n * factorial_rec(n - 1); 
}

int main() {
    int n;
    printf("Enter a non-negative integer: ");
    scanf("%d", &n);
    printf("%d! = %lld\n", n, factorial_rec(n));
    return 0;
}
