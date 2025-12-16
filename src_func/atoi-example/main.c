#include <stdio.h> // Для printf
#include <ctype.h> // Для isspace, isdigit

int custom_atoi(const char *str) {
    int result = 0;
    int sign = 1; // Знак числа: 1 для положительного, -1 для отрицательного
    int i = 0;    // Индекс для перебора строки

    // 1. Пропуск начальных пробельных символов (whitespace)
    while (isspace((unsigned char)str[i])) {
        i++;
    }

    // 2. Определение знака
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // 3. Преобразование цифр
    // Идем по строке, пока встречаем цифры
    while (isdigit((unsigned char)str[i])) {
        // Преобразуем символ цифры в числовое значение
        int digit = str[i] - '0';
        // Добавляем цифру к результату (с учетом разряда)
        result = result * 10 + digit;
        i++;
    }

    // 4. Применение знака и возврат результата
    return result * sign;
}

int main() {
    char str1[] = "   -12345";
    char str2[] = "42";
    char str3[] = "  +987abc";
    char str4[] = "hello world";
    char str5[] = "";

    printf("'%s' -> %d\n", str1, custom_atoi(str1)); // -12345
    printf("'%s' -> %d\n", str2, custom_atoi(str2)); // 42
    printf("'%s' -> %d\n", str3, custom_atoi(str3)); // 987
    printf("'%s' -> %d\n", str4, custom_atoi(str4)); // 0
    printf("'%s' -> %d\n", str5, custom_atoi(str5)); // 0

    return 0;
}
