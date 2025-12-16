/* Реализация strchr в Си сводится к простому перебору
 * строки символ за символом с помощью указателя или
 * цикла for, пока не будет найден искомый символ или
 * достигнут конец строки (\0), возвращая указатель на
 * найденное вхождение или NULL, если символ не найден.
 * Это классический пример работы с указателями и
 * строками в Си */

#include <stdio.h> // Для printf
#include <stddef.h> // Для NULL

char* my_strchr(const char* str, int character) {
    // Перебираем строку до тех пор, пока не встретим '\0' (конец строки)
    while (*str != '\0') {
        // Если текущий символ совпадает с искомым
        if (*str == (char)character) {
            return (char*)str; // Возвращаем указатель на этот символ
        }
        str++; // Переходим к следующему символу
    }

    // Если символ '\0' совпадает с искомым (обработка поиска '\0')
    if ((char)character == '\0') {
        return (char*)str; // Возвращаем указатель на '\0'
    }

    return NULL; // Символ не найден
}

int main() {
    const char* text = "Hello, world!";
    char* found;

    // Ищем 'w'
    found = my_strchr(text, 'w');
    if (found != NULL) {
        printf("Найден символ '%c' в позиции: %ld\n", 'w', found - text); // Показываем смещение
    } else {
        printf("Символ 'w' не найден.\n");
    }

    // Ищем 'z'
    found = my_strchr(text, 'z');
    if (found != NULL) {
        printf("Найден символ '%c' в позиции: %ld\n", 'z', found - text);
    } else {
        printf("Символ 'z' не найден.\n");
    }

    // Ищем '\0'
    found = my_strchr(text, '\0');
    if (found != NULL) {
        printf("Найден завершающий нулевой символ '\\0' в позиции: %ld\n", found - text);
    }

    return 0;
}
