#include <stdlib.h>

#include "bmp.h"

// int save_buffer_to_bmp(const char* filename, unsigned char* buffer, int width, int height)
// {
//     FILE* file = fopen(filename, "wb"); // Открываем файл в бинарном режиме для записи
//     if (!file) {
//         printf("Ошибка: Не удалось создать файл %s\n", filename);
//         return -1;
//     }

//     // Вычисляем размер строки с учетом выравнивания до 4 байт
//     int rowPadded = (width * 3 + 3) & (~3);
//     uint32_t imageSize = rowPadded * abs(height);
//     uint32_t fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;

//     // --- Заполнение BITMAPFILEHEADER ---
//     BITMAPFILEHEADER fileHeader;
//     fileHeader.bfType = 0x4D42;      // 'BM'
//     fileHeader.bfSize = fileSize;
//     fileHeader.bfReserved1 = 0;
//     fileHeader.bfReserved2 = 0;
//     fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

//     // --- Заполнение BITMAPINFOHEADER ---
//     BITMAPINFOHEADER infoHeader;
//     infoHeader.biSize = sizeof(BITMAPINFOHEADER);
//     infoHeader.biWidth = width;
//     infoHeader.biHeight = height; // Положительная высота означает, что изображение хранится снизу вверх
//     infoHeader.biPlanes = 1;
//     infoHeader.biBitCount = 24;   // 24 бита на пиксель
//     infoHeader.biCompression = 0; // BI_RGB (без сжатия)
//     infoHeader.biSizeImage = imageSize;
//     infoHeader.biXPelsPerMeter = 0;
//     infoHeader.biYPelsPerMeter = 0;
//     infoHeader.biClrUsed = 0;
//     infoHeader.biClrImportant = 0;

//     // --- Запись заголовков в файл ---
//     fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
//     fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

//     // --- Запись данных изображения с учетом выравнивания ---

//     // Если данные в буфере уже выровнены, можно записать их одной операцией.
//     // Если буфер содержит "плоские" данные без паддинга,
//     // нужно итерироваться по строкам и добавлять паддинг вручную.
//     // В данном примере мы предполагаем, что буфер *уже* имеет нужный размер
//     // с учетом паддинга, как было рассчитано в функции загрузки.

//     if (width * 3 == rowPadded) {
//         // Оптимизированная запись, если паддинг не нужен (ширина кратна 4)
//         fwrite(buffer, 1, imageSize, file);
//     } else {
//         // Если паддинг нужен, записываем построчно
//         unsigned char padding[3] = {0, 0, 0};
//         int paddingSize = rowPadded - (width * 3);

//         for (int i = 0; i < abs(height); i++) {
//             // Записываем строку данных
//             fwrite(buffer + i * (width * 3), 1, width * 3, file);
//             // Записываем паддинг
//             fwrite(padding, 1, paddingSize, file);
//         }
//     }

//     fclose(file);
//     printf("Файл %s успешно сохранен.\n", filename);
//     return 0;
// }

// //********************************************************
// unsigned char* crop_buffer_roi(
//     unsigned char* srcBuffer, int srcWidth, int srcHeight,
//     int startX, int startY, int cropWidth, int cropHeight,
//     int* destWidth, int* destHeight) {

//     // Проверки на корректность координат и размеров
//     if (startX < 0 || startY < 0 || startX + cropWidth > srcWidth || startY + cropHeight > srcHeight || cropWidth <= 0 || cropHeight <= 0) {
//         printf("Ошибка: Некорректные параметры фрагмента ROI.\n");
//         return NULL;
//     }

//     *destWidth = cropWidth;
//     *destHeight = cropHeight;

//     // Рассчитываем шаг строки (stride) для исходного и нового буферов, учитывая выравнивание BMP
//     int srcStride = (srcWidth * 3 + 3) & (~3);
//     int destStride = (cropWidth * 3 + 3) & (~3);
//     uint32_t destImageSize = destStride * abs(cropHeight);

//     // Выделяем память под новый буфер
//     unsigned char* destBuffer = (unsigned char*)malloc(destImageSize);
//     if (!destBuffer) {
//         printf("Ошибка: Недостаточно памяти для нового буфера.\n");
//         return NULL;
//     }

//     // Копируем данные построчно
//     for (int i = 0; i < abs(cropHeight); i++) {
//         // Рассчитываем смещение (offset) для текущей строки в исходном буфере:
//         // startY * srcStride дает начало нужной строки в исходнике.
//         // startX * 3 дает смещение до нужного пикселя в этой строке (3 байта на пиксель).
//         // (srcHeight - 1 - startY - i) * srcStride - альтернативный расчет для bottom-up BMP

//         // Для простоты примера, будем считать, что изображения "верх-вниз" в памяти или мы корректно обрабатываем bottom-up:
//         // Если srcHeight > 0 (bottom-up BMP):
//         int srcRowOffset = (startY + i) * srcStride + startX * 3;
//         // Если srcHeight < 0 (top-down BMP):
//         // int srcRowOffset = abs(startY + i) * srcStride + startX * 3;

//         // Рассчитываем смещение для текущей строки в новом буфере (всегда top-down при копировании в flat buffer)
//         int destRowOffset = i * destStride;

//         // Копируем cropWidth * 3 байт из исходной строки в новую
//         memcpy(destBuffer + destRowOffset, srcBuffer + srcRowOffset, cropWidth * 3);

//         // Примечание: memcpy копирует только нужные данные. Паддинг в конце строки нового буфера
//         // уже учтен выделенным размером destImageSize и остается нулевым (если память была инициализирована)
//         // или просто не является проблемой для функции save_buffer_to_bmp, если она корректно пишет паддинг.
//     }

//     return destBuffer;
// }
// //***************************************************************
// /**
//  * Переворачивает изображение в буфере по горизонтали (зеркально по вертикальной оси).
//  * Работает inplace (модифицирует исходный буфер).
//  *
//  * @param buffer Буфер пикселей изображения (24-бит BGR).
//  * @param width Ширина изображения.
//  * @param height Высота изображения.
//  */
// void flip_buffer_horizontal(unsigned char* buffer, int width, int height) {
//     int stride = (width * 3 + 3) & (~3);
//     int absHeight = abs(height);
//     int bytesPerPixel = 3;

//     for (int i = 0; i < absHeight; i++) {
//         unsigned char* currentRow = buffer + i * stride;

//         // Итерируемся только до середины строки (width / 2 пикселей)
//         for (int j = 0; j < width / 2; j++) {
//             // Определяем указатели на левый и правый пиксель
//             unsigned char* leftPixel = currentRow + j * bytesPerPixel;
//             unsigned char* rightPixel = currentRow + (width - 1 - j) * bytesPerPixel;

//             // Меняем местами 3 байта (BGR) для двух пикселей
//             unsigned char temp[3];
//             memcpy(temp, leftPixel, bytesPerPixel);
//             memcpy(leftPixel, rightPixel, bytesPerPixel);
//             memcpy(rightPixel, temp, bytesPerPixel);
//         }
//     }
//     printf("Изображение перевернуто по горизонтали.\n");
// }

// /**
//  * Переворачивает изображение в буфере по вертикали (зеркально по горизонтальной оси).
//  * Работает inplace (модифицирует исходный буфер).
//  *
//  * @param buffer Буфер пикселей изображения (24-бит BGR).
//  * @param width Ширина изображения.
//  * @param height Высота изображения.
//  */
// void flip_buffer_vertical(unsigned char* buffer, int width, int height) {
//     // Рассчитываем шаг строки (stride) с учетом выравнивания до 4 байт
//     int stride = (width * 3 + 3) & (~3);
//     int absHeight = abs(height); // Работаем с абсолютной высотой

//     // Выделяем временную память для хранения одной строки
//     unsigned char* tempRow = (unsigned char*)malloc(stride);
//     if (!tempRow == NULL) {
//         printf("Ошибка выделения памяти для временной строки.\n");
//         return;
//     }

//     // Итерируемся только до середины изображения
//     for (int i = 0; i < absHeight / 2; i++) {
//         // Определяем смещение для верхней строки
//         unsigned char* topRow = buffer + i * stride;
//         // Определяем смещение для нижней строки
//         unsigned char* bottomRow = buffer + (absHeight - 1 - i) * stride;

//         // Меняем местами верхнюю и нижнюю строки через временный буфер
//         memcpy(tempRow, topRow, stride);
//         memcpy(topRow, bottomRow, stride);
//         memcpy(bottomRow, tempRow, stride);
//     }

//     free(tempRow);
//     printf("Изображение перевернуто по вертикали.\n");
// }


int main()
{
  char *in_file_name = "960px-Lenna.bmp";
  char *out_file_name = "croup_lenna.bmp";
  int cropX = 70;
  int cropY = 70;
  int cropW = 480;
  int cropH = 680;
  int srcWidth, srcHeight;

  unsigned char *pixel_data = load_bmp(in_file_name, &srcWidth, &srcHeight);

  unsigned char* croppedBuffer = crop(pixel_data, srcWidth, srcHeight, cropX, cropY, cropW, cropH);

  save_bmp(out_file_name, croppedBuffer, cropW, cropH);

  free(pixel_data);
  free(croppedBuffer);



  //***************************************************************
  // РАБОТАЮЩИЙ КОД
  // FILE* source_file_bmp;

  // BITMAPFILEHEADER fileHeader;
  // BITMAPINFOHEADER infoHeader;

  // if((source_file_bmp = fopen("960px-Lenna.bmp","rb")) == NULL)
  //   return -1;

  // // Чтение заголовка файла
  //   if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, source_file_bmp) != 1) {
  //       perror("Ошибка чтения BITMAPFILEHEADER");
  //       fclose(source_file_bmp);
  //       return 0;
  //   }

  // printf("bfSize: %d\n", fileHeader.bfSize);
  // printf("bfOffBits: %d\n", fileHeader.bfOffBits);

  //   // Проверка сигнатуры 'BM' (в little-endian это 0x4D42)
  //   if (fileHeader.bfType != 0x4D42) {
  //       fprintf(stderr, "Файл не является BMP-изображением.\n");
  //       fclose(source_file_bmp);
  //       return 0;
  //   }

  //   // Чтение информационного заголовка
  //   if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, source_file_bmp) != 1) {
  //       perror("Ошибка чтения BITMAPINFOHEADER");
  //       fclose(source_file_bmp);
  //       return 0;
  //   }

  //   printf("Смещение до данных изображения: %d\n", fileHeader.bfOffBits);
  //   // Вывод базовой информации
  //   printf("Ширина: %d, Высота: %d\n", infoHeader.biWidth, infoHeader.biHeight);
  //   printf("Глубина цвета (бит/пиксель): %d\n", infoHeader.biBitCount);

  //   // Определяем размер изображения
  //   // Обычно это ширина * высота * количество каналов
  //   int width = infoHeader.biWidth;
  //   int height = infoHeader.biHeight;
  //   int num_bytes = width * height * 3; // 3 для RGB

  //   unsigned char *pixel_data = (unsigned char *)malloc(num_bytes);

  // if (pixel_data == NULL) {
  //   // Обработка ошибки
  //   fclose(source_file_bmp);
  //   return -1;
  // }

  // // Установка указателя файла на начало данных пикселей
  // fseek(source_file_bmp, fileHeader.bfOffBits, SEEK_SET);

  // // читаем в буфер
  // fread(pixel_data, sizeof(unsigned char), num_bytes, source_file_bmp);


  //   int cropX = 70;
  //   int cropY = 70;
  //   int cropW = 400;
  //   int cropH = 400;

  //   int destW, destH;
  //   unsigned char* croppedBuffer = crop_buffer_roi(
  //       pixel_data, width, height,
  //       cropX, cropY, cropW, cropH,
  //       &destW, &destH
  //   );

  // flip_buffer_horizontal(croppedBuffer, cropW, cropH);

  // //flip_buffer_vertical(croppedBuffer, cropW, cropH); неработит

  // if (save_buffer_to_bmp("output_crop.bmp", croppedBuffer, cropW, cropH) == 0) {
  //       printf("Создан файл output_crop.bmp\n");
  //   }

  //  free(croppedBuffer);

  // if (save_buffer_to_bmp("output.bmp", pixel_data, width, height) == 0) {
  //       printf("Создан файл output.bmp\n");
  //   }

  // fclose(source_file_bmp);

  return 0;
}























// Функция для вырезания фрагмента (ROI)
// c

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <string.h>

// /**
//  * Вырезает прямоугольный фрагмент (ROI) из исходного буфера в новый буфер.
//  *
//  * @param srcBuffer Исходный буфер изображения (24-бит BGR, с учетом паддинга).
//  * @param srcWidth Ширина исходного изображения.
//  * @param srcHeight Высота исходного изображения.
//  * @param startX Начальная X-координата вырезаемого фрагмента.
//  * @param startY Начальная Y-координата вырезаемого фрагмента.
//  * @param cropWidth Ширина вырезаемого фрагмента.
//  * @param cropHeight Высота вырезаемого фрагмента.
//  * @param destWidth Указатель для записи ширины нового изображения.
//  * @param destHeight Указатель для записи высоты нового изображения.
//  * @return Новый буфер с фрагментом изображения (должен быть освобожден через free()).
//  */
// unsigned char* crop_buffer_roi(
//     unsigned char* srcBuffer, int srcWidth, int srcHeight,
//     int startX, int startY, int cropWidth, int cropHeight,
//     int* destWidth, int* destHeight) {

//     // Проверки на корректность координат и размеров
//     if (startX < 0 || startY < 0 || startX + cropWidth > srcWidth || startY + cropHeight > srcHeight || cropWidth <= 0 || cropHeight <= 0) {
//         printf("Ошибка: Некорректные параметры фрагмента ROI.\n");
//         return NULL;
//     }

//     *destWidth = cropWidth;
//     *destHeight = cropHeight;

//     // Рассчитываем шаг строки (stride) для исходного и нового буферов, учитывая выравнивание BMP
//     int srcStride = (srcWidth * 3 + 3) & (~3);
//     int destStride = (cropWidth * 3 + 3) & (~3);
//     uint32_t destImageSize = destStride * abs(cropHeight);

//     // Выделяем память под новый буфер
//     unsigned char* destBuffer = (unsigned char*)malloc(destImageSize);
//     if (!destBuffer) {
//         printf("Ошибка: Недостаточно памяти для нового буфера.\n");
//         return NULL;
//     }

//     // Копируем данные построчно
//     for (int i = 0; i < abs(cropHeight); i++) {
//         // Рассчитываем смещение (offset) для текущей строки в исходном буфере:
//         // startY * srcStride дает начало нужной строки в исходнике.
//         // startX * 3 дает смещение до нужного пикселя в этой строке (3 байта на пиксель).
//         // (srcHeight - 1 - startY - i) * srcStride - альтернативный расчет для bottom-up BMP

//         // Для простоты примера, будем считать, что изображения "верх-вниз" в памяти или мы корректно обрабатываем bottom-up:
//         // Если srcHeight > 0 (bottom-up BMP):
//         int srcRowOffset = (startY + i) * srcStride + startX * 3;
//         // Если srcHeight < 0 (top-down BMP):
//         // int srcRowOffset = abs(startY + i) * srcStride + startX * 3;

//         // Рассчитываем смещение для текущей строки в новом буфере (всегда top-down при копировании в flat buffer)
//         int destRowOffset = i * destStride;

//         // Копируем cropWidth * 3 байт из исходной строки в новую
//         memcpy(destBuffer + destRowOffset, srcBuffer + srcRowOffset, cropWidth * 3);

//         // Примечание: memcpy копирует только нужные данные. Паддинг в конце строки нового буфера
//         // уже учтен выделенным размером destImageSize и остается нулевым (если память была инициализирована)
//         // или просто не является проблемой для функции save_buffer_to_bmp, если она корректно пишет паддинг.
//     }

//     return destBuffer;
// }


// // Пример использования:
// int main() {
//     // Предположим, у нас есть буфер 100x100 пикселей (srcBuffer, srcW=100, srcH=100)
//     // Мы хотим вырезать фрагмент 20x30, начиная с координат (10, 10)
//     int srcW = 100, srcH = 100;
//     // ... код создания или загрузки srcBuffer ...
//     // Заглушка:
//     unsigned char* srcBuffer = (unsigned char*)malloc(((srcW * 3 + 3) & (~3)) * srcH);
//     if (!srcBuffer) return 1;
//     // Заполняем srcBuffer какими-то данными...

//     int cropX = 10;
//     int cropY = 10;
//     int cropW = 20;
//     int cropH = 30;

//     int destW, destH;
//     unsigned char* croppedBuffer = crop_buffer_roi(
//         srcBuffer, srcW, srcH,
//         cropX, cropY, cropW, cropH,
//         &destW, &destH
//     );

//     if (croppedBuffer) {
//         printf("Фрагмент успешно вырезан. Новые размеры: %dx%d\n", destW, destH);
//         // Теперь croppedBuffer можно сохранить в новый BMP-файл, используя функцию save_buffer_to_bmp() из предыдущего ответа

//         // Обязательно освобождаем память
//         free(croppedBuffer);
//     }

//     free(srcBuffer); // Освобождаем исходный буфер
//     return 0;
// }



// https://www.linux.org.ru/forum/general/12278746
// https://www.cyberforum.ru/c-beginners/thread1263551.html

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // Структура для хранения информации о BMP
// typedef struct {
//     int width;
//     int height;
//     int bitsPerPixel;
//     unsigned char *data;
// } BMPImage;

// int main() {
//     FILE *file;
//     BMPImage image;
//     unsigned char header[54]; // Стандартный размер заголовка BMP

//     // Открываем файл
//     file = fopen("image.bmp", "rb");
//     if (file == NULL) {
//         fprintf(stderr, "Не удалось открыть файл\n");
//         return 1;
//     }

//     // Читаем заголовок
//     fread(header, sizeof(unsigned char), 54, file);

//     // Извлекаем информацию из заголовка (нумерация байт может отличаться в зависимости от версии BMP)
//     // Примерное извлечение:
//     image.width = *(int*)&header[18];
//     image.height = *(int*)&header[22];
//     image.bitsPerPixel = *(short*)&header[28];
//     int pixelDataOffset = *(int*)&header[10];

//     // Проверяем корректность данных
//     if (image.width <= 0 || image.height <= 0 || image.bitsPerPixel != 24) {
//         fprintf(stderr, "Неподдерживаемый формат BMP\n");
//         fclose(file);
//         return 1;
//     }

//     // Перемещаемся к началу данных пикселей
//     fseek(file, pixelDataOffset, SEEK_SET);

//     // Выделяем память под пиксели (для 24-битного BMP)
//     int bytesPerPixel = image.bitsPerPixel / 8;
//     int rowSize = image.width * bytesPerPixel;
//     // Для BMP может быть выравнивание в конце каждой строки, поэтому добавим padding
//     int padding = (4 - (rowSize % 4)) % 4;
//     rowSize += padding;
//     image.data = (unsigned char *)malloc(image.width * image.height * bytesPerPixel);

//     if (image.data == NULL) {
//         fprintf(stderr, "Ошибка выделения памяти\n");
//         fclose(file);
//         return 1;
//     }

//     // Читаем данные пикселей
//     for (int i = 0; i < image.height; i++) {
//         fread(image.data + (image.height - 1 - i) * image.width * bytesPerPixel, sizeof(unsigned char), rowSize, file);
//     }

//     // Теперь `image.data` содержит данные пикселей. Можно работать с ними.
//     // Например, получить доступ к пикселю (x, y):
//     // image.data[ (y * image.width + x) * bytesPerPixel ] - это синий канал
//     // image.data[ (y * image.width + x) * bytesPerPixel + 1] - это зеленый канал
//     // image.data[ (y * image.width + x) * bytesPerPixel + 2] - это красный канал

//     // Освобождаем память
//     free(image.data);
//     fclose(file);

//     return 0;
// }
// ========================================================================

// #include <stdio.h>
// #include <stdlib.h>

// int main() {
//     FILE *input_file, *output_file;
//     unsigned char header[54]; // Для заголовка BMP-файла
//     int width, height;

//     // Открываем файл для чтения
//     input_file = fopen("input.bmp", "rb");
//     if (input_file == NULL) {
//         perror("Ошибка открытия входного файла");
//         return 1;
//     }

//     // Читаем заголовок
//     fread(header, 1, 54, input_file);
//     // ... извлекаем width и height из заголовка ...

//     // Открываем новый файл для записи
//     output_file = fopen("output.bmp", "wb");
//     if (output_file == NULL) {
//         perror("Ошибка открытия выходного файла");
//         fclose(input_file);
//         return 1;
//     }

//     // Записываем заголовок в новый файл
//     fwrite(header, 1, 54, output_file);

//     // ... считываем данные пикселей, обрабатываем и записываем в output_file ...

//     // Закрываем файлы
//     fclose(input_file);
//     fclose(output_file);

//     return 0;
// }
