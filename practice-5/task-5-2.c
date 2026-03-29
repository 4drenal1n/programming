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
