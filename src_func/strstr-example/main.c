/* Реализация strstr на C включает два указателя для
 * основной строки и искомой подстроки; она проходит
 * по главной строке, и при совпадении первого символа
 * начинает проверять всю подстроку, возвращая указатель
 * на начало совпадения или NULL, если подстрока не найдена,
 * причем пустая искомая строка возвращает указатель
 * на начало основной строки */

#include <stdio.h> // Для printf
#include <stddef.h> // Для NULL

char *my_strstr(const char *haystack, const char *needle) {
    // Если искомая строка пустая, возвращаем начало основной строки
    if (*needle == '\0') {
        return (char *)haystack;
    }

    // Внешний цикл: перебираем основную строку (haystack)
    while (*haystack != '\0') {
        const char *h_ptr = haystack; // Указатель для сравнения
        const char *n_ptr = needle;   // Указатель на искомую строку

        // Внутренний цикл: проверяем совпадение подстроки
        // Пока символы совпадают и не достигли конца иголки (needle)
        while (*h_ptr == *n_ptr && *n_ptr != '\0') {
            h_ptr++;
            n_ptr++;
        }

        // Если достигли конца иголки, значит, подстрока найдена
        if (*n_ptr == '\0') {
            return (char *)haystack; // Возвращаем указатель на начало совпадения
        }

        // Переходим к следующему символу в основной строке
        haystack++;
    }

    // Если цикл завершился, а совпадений нет
    return NULL;
    }

int main() {
    const char *text = "Hello, world! This is a C example.";
    const char *sub1 = "world";
    const char *sub2 = "C example";
    const char *sub3 = "not found";
    const char *sub4 = ""; // Пустая подстрока

    char *result1 = my_strstr(text, sub1);
    if (result1) {
        printf("Подстрока \"%s\" найдена в \"%s\" по адресу: %p\n", sub1, text, (void *)result1);
    } else {
        printf("Подстрока \"%s\" не найдена\n", sub1);
    }

    char *result2 = my_strstr(text, sub2);
    if (result2) {
        printf("Подстрока \"%s\" найдена в \"%s\" по адресу: %p\n", sub2, text, (void *)result2);
    }

    char *result3 = my_strstr(text, sub3);
    if (result3 == NULL) {
        printf("Подстрока \"%s\" не найдена (ожидаемо)\n", sub3);
    }

    char *result4 = my_strstr(text, sub4);
    if (result4) {
        printf("Пустая подстрока найдена по адресу: %p (должен быть адрес текста)\n", (void *)result4);
    }

    return 0;
}
