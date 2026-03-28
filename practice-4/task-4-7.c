#include <stdio.h>

// Функция вычисления длины строки (без использования strlen)
int my_strlen(char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int main() {
    // Примеры из условия
    printf("Length: %d\n", my_strlen("Hello"));  // 5
    printf("Length: %d\n", my_strlen(""));       // 0
    printf("Length: %d\n", my_strlen(" "));      // 1 (пробел - символ)

    
    char str[100];
    printf("\nEnter a string: ");
    fgets(str, sizeof(str), stdin); 

    
    int len = my_strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--; 
    }

    printf("Length of \"%s\": %d\n", str, len);

    return 0;
}
