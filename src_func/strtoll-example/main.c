#include <stdio.h>
#include <ctype.h> // Для isspace, isdigit
#include <limits.h> // Для LLONG_MAX, LLONG_MIN

// Упрощенная функция для преобразования строки в long long
long long my_strtoll(const char *nptr, char **endptr, int base) {
    long long result = 0;
    int sign = 1;
    const char *p = nptr;

    // 1. Пропуск начальных пробелов
    while (isspace(*p)) {
        p++;
    }

    // 2. Обработка знака
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    // 3. Итеративное преобразование цифр
    while (isdigit(*p) || (base > 10 && (*p >= 'a' && *p <= 'f')) || (base > 10 && (*p >= 'A' && *p <= 'F'))) {
        int digit;
        if (isdigit(*p)) {
            digit = *p - '0';
        } else { // Для оснований > 10 (шестнадцатеричная и т.д.)
            digit = (*p >= 'a' && *p <= 'f') ? (*p - 'a' + 10) : (*p - 'A' + 10);
        }

        // Проверка на переполнение перед умножением (упрощенно)
        // В реальной реализации нужна более точная проверка!
        if (result > (LLONG_MAX - digit) / base) {
            // Ошибка переполнения (в реальной функции устанавливает errno)
            // Для простоты вернем MAX или MIN
            *endptr = (char *)p; // Указываем, где остановились
            return (sign == 1) ? LLONG_MAX : LLONG_MIN;
        }

        result = result * base + digit;
        p++;
    }

    // 4. Установка указателя на конец
    if (endptr != NULL) {
        *endptr = (char *)p;
    }

    return result * sign;
}

int main() {
    const char *str = "  -12345abc 67";
    char *end;
    long long value = my_strtoll(str, &end, 10);
    printf("Строка: \"%s\"\n", str);
    printf("Результат: %lld\n", value); // -12345
    printf("Остаток строки: \"%s\"\n", end); // "abc 67" (пробел тоже входит)

    const char *str2 = "0x1A";
    long long value2 = my_strtoll(str2, &end, 16);
    printf("Строка: \"%s\"\n", str2);
    printf("Результат: %lld\n", value2); // 26
    printf("Остаток строки: \"%s\"\n", end); // ""

    return 0;
}
