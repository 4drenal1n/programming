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
