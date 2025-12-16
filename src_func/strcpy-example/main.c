#include <stdio.h>

char* my_strcpy(char* dest, const char* src) {
    char* original_dest = dest; // Сохраняем начальный адрес назначения

    // Копируем символы, пока не дойдем до нулевого терминатора
    while (*src != '\0') {
        *dest = *src; // Копируем текущий символ
        dest++;       // Переходим к следующему символу в назначении
        src++;        // Переходим к следующему символу в источнике
    }
    // Копируем сам нулевой терминатор
    *dest = '\0';

    return original_dest; // Возвращаем указатель на начало строки назначения
}

int main() {
    char source[] = "Hello, World!";
    char destination[20]; // Убедитесь, что размер достаточен!

    // Использование нашей функции
    my_strcpy(destination, source);

    printf("Исходная строка: %s\n", source);
    printf("Скопированная строка: %s\n", destination);

    return 0;
}
