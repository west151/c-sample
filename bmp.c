#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

// загрузка файла (пиксели) в память
unsigned char* load_bmp(char *file_name, int *sourceWidth, int *sourceHeight)
{
  FILE* source_file_bmp;
  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
  unsigned char *buffer;

  if((source_file_bmp = fopen(file_name,"rb")) == NULL)
    return NULL;

  fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, source_file_bmp);

  printf("bfSize: %d\n", fileHeader.bfSize);
  printf("bfOffBits: %d\n", fileHeader.bfOffBits);

  // Проверка сигнатуры 'BM' (в little-endian это 0x4D42)
  if (fileHeader.bfType != 0x4D42) {
    fprintf(stderr, "Файл не является BMP-изображением.\n");
    fclose(source_file_bmp);
    return 0;
  }

  fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, source_file_bmp);

  printf("Смещение до данных изображения: %d\n", fileHeader.bfOffBits);
  printf("Ширина: %d, Высота: %d\n", infoHeader.biWidth, infoHeader.biHeight);
  printf("Глубина цвета (бит/пиксель): %d\n", infoHeader.biBitCount);

  *sourceWidth = infoHeader.biWidth;
  *sourceHeight = infoHeader.biHeight;

  // Установка указателя файла на начало данных пикселей
  fseek(source_file_bmp, fileHeader.bfOffBits, SEEK_SET);

  int width = infoHeader.biWidth;
  int height = infoHeader.biHeight;
  int num_bytes = width * height * 3; // 3 для RGB

  buffer = (unsigned char *)malloc(num_bytes);
  fread(buffer, sizeof(unsigned char), num_bytes, source_file_bmp);

  fclose(source_file_bmp);

  return buffer;
}

int save_bmp(char *file_name, unsigned char *buffer, int width, int height)
{
    FILE* target_file_bmp = fopen(file_name, "wb");

    if (!target_file_bmp) {
        printf("Ошибка: Не удалось создать файл %s\n", file_name);
        return -1;
    }

    // Вычисляем размер строки с учетом выравнивания до 4 байт
    int rowPadded = (width * 3 + 3) & (~3);
    uint32_t imageSize = rowPadded * abs(height);
    uint32_t fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;

    // --- Заполнение BITMAPFILEHEADER ---
    BITMAPFILEHEADER fileHeader;
    fileHeader.bfType = 0x4D42;      // 'BM'
    fileHeader.bfSize = fileSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // --- Заполнение BITMAPINFOHEADER ---
    BITMAPINFOHEADER infoHeader;
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height; // Положительная высота означает, что изображение хранится снизу вверх
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;   // 24 бита на пиксель
    infoHeader.biCompression = 0; // BI_RGB (без сжатия)
    infoHeader.biSizeImage = imageSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    // --- Запись заголовков в файл ---
    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, target_file_bmp);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, target_file_bmp);

    // --- Запись данных изображения с учетом выравнивания ---

    // Если данные в буфере уже выровнены, можно записать их одной операцией.
    // Если буфер содержит "плоские" данные без паддинга,
    // нужно итерироваться по строкам и добавлять паддинг вручную.
    // В данном примере мы предполагаем, что буфер *уже* имеет нужный размер
    // с учетом паддинга, как было рассчитано в функции загрузки.

    if (width * 3 == rowPadded) {
        // Оптимизированная запись, если паддинг не нужен (ширина кратна 4)
        fwrite(buffer, 1, imageSize, target_file_bmp);
    } else {
        // Если паддинг нужен, записываем построчно
        unsigned char padding[3] = {0, 0, 0};
        int paddingSize = rowPadded - (width * 3);

        for (int i = 0; i < abs(height); i++) {
            // Записываем строку данных
            fwrite(buffer + i * (width * 3), 1, width * 3, target_file_bmp);
            // Записываем паддинг
            fwrite(padding, 1, paddingSize, target_file_bmp);
        }
    }

    fclose(target_file_bmp);
    printf("Файл %s успешно сохранен.\n", file_name);

  return 0;
}

unsigned char* crop(unsigned char* srcBuffer, int srcWidth, int srcHeight, int startX, int startY, int cropWidth, int cropHeight)
{
    // Проверки на корректность координат и размеров
    if (startX < 0 || startY < 0 || startX + cropWidth > srcWidth || startY + cropHeight > srcHeight || cropWidth <= 0 || cropHeight <= 0) {
        printf("Ошибка: Некорректные параметры фрагмента ROI.\n");
        return NULL;
    }

    // Рассчитываем шаг строки (stride) для исходного и нового буферов, учитывая выравнивание BMP
    int srcStride = (srcWidth * 3 + 3) & (~3);
    int destStride = (cropWidth * 3 + 3) & (~3);
    uint32_t destImageSize = destStride * abs(cropHeight);

    // Выделяем память под новый буфер
    unsigned char* destBuffer = (unsigned char*)malloc(destImageSize);
    if (!destBuffer) {
        printf("Ошибка: Недостаточно памяти для нового буфера.\n");
        return NULL;
    }

    // Копируем данные построчно
    for (int i = 0; i < abs(cropHeight); i++) {
        // Рассчитываем смещение (offset) для текущей строки в исходном буфере:
        // startY * srcStride дает начало нужной строки в исходнике.
        // startX * 3 дает смещение до нужного пикселя в этой строке (3 байта на пиксель).
        // (srcHeight - 1 - startY - i) * srcStride - альтернативный расчет для bottom-up BMP

        // Для простоты примера, будем считать, что изображения "верх-вниз" в памяти или мы корректно обрабатываем bottom-up:
        // Если srcHeight > 0 (bottom-up BMP):
        int srcRowOffset = (startY + i) * srcStride + startX * 3;
        // Если srcHeight < 0 (top-down BMP):
        // int srcRowOffset = abs(startY + i) * srcStride + startX * 3;

        // Рассчитываем смещение для текущей строки в новом буфере (всегда top-down при копировании в flat buffer)
        int destRowOffset = i * destStride;

        // Копируем cropWidth * 3 байт из исходной строки в новую
        memcpy(destBuffer + destRowOffset, srcBuffer + srcRowOffset, cropWidth * 3);

        // Примечание: memcpy копирует только нужные данные. Паддинг в конце строки нового буфера
        // уже учтен выделенным размером destImageSize и остается нулевым (если память была инициализирована)
        // или просто не является проблемой для функции save_buffer_to_bmp, если она корректно пишет паддинг.
    }

    return destBuffer;
}

unsigned char *rotate(unsigned char *srcCropBuffer, int cropWidthBuffer, int cropHeightBuffer)
{
  PIXEL* pixel_array = (PIXEL*)srcCropBuffer;

  int new_width = cropHeightBuffer; // Ширина и высота меняются местами
  int new_height = cropWidthBuffer;
  PIXEL *dst_data = malloc((new_width * new_height + new_height) * sizeof(PIXEL));

  // int padding_out = (4 - (new_width * sizeof(PIXEL)) % 4) % 4;
  // PIXEL *dst_data = malloc((new_width * new_height + new_height * padding_out) * sizeof(PIXEL));

  rotate_pixels(pixel_array, dst_data, cropHeightBuffer, cropWidthBuffer); // Передаем height как rows, width как cols

  unsigned char* result = (unsigned char*)dst_data;
}

void rotate_pixels(PIXEL *src_data, PIXEL *dst_data, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Новая строка = старый столбец
            int new_r = c;
            // Новый столбец = старая высота - 1 - старая строка
            int new_c = rows - 1 - r;

            int src_idx = r * cols + c;
            int dst_idx = new_r * rows + new_c; // Ширина нового изображения = исходная высота (rows)

            dst_data[dst_idx] = src_data[src_idx];
        }
    }
}







// void flip_buffer_horizontal(unsigned char* buffer, int width, int height)
// {
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

// void flip_buffer_vertical(unsigned char* buffer, int width, int height) {
//     // Рассчитываем шаг строки (stride) с учетом выравнивания до 4 байт
//     int stride = (width * 3 + 3) & (~3);
//     int absHeight = abs(height); // Работаем с абсолютной высотой

//     // Выделяем временную память для хранения одной строки
//     unsigned char* tempRow = (unsigned char*)malloc(stride);
//     if ( tempRow == NULL) {
//         printf("Ошибка выделения памяти для временной строки (flip_buffer_vertical).\n");
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
