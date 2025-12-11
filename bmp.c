#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// загрузка файла (пиксели) в память
unsigned char* load_bmp(char *file_name, uint32_t *source_width, uint32_t *source_height)
{
  FILE* source_file_bmp;
  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
  unsigned char *buffer;

  memset(&fileHeader, 0, sizeof(fileHeader));
  memset(&infoHeader, 0, sizeof(infoHeader));

  if((source_file_bmp = fopen(file_name,"rb")) == NULL)
    return NULL;

  fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, source_file_bmp);

  printf("bfSize: %d\n", fileHeader.bfSize);
  printf("bfOffBits: %d\n", fileHeader.bfOffBits);

  // Проверка сигнатуры 'BM' (в little-endian это 0x4D42)
  if (fileHeader.bfType != 0x4D42) {
    printf("Файл %s не является BMP-изображением.\n", file_name);
    fclose(source_file_bmp);
    return NULL;
  }

  fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, source_file_bmp);

  printf("Смещение до данных изображения: %d\n", fileHeader.bfOffBits);
  printf("Ширина: %d, Высота: %d\n", infoHeader.biWidth, infoHeader.biHeight);
  printf("Глубина цвета (бит/пиксель): %d\n", infoHeader.biBitCount);

  printf("infoHeader.biSizeImage: %d\n", infoHeader.biSizeImage);
  printf("infoHeader.biPlanes: %d\n", infoHeader.biPlanes);
  printf("infoHeader.biXPelsPerMeter: %d\n", infoHeader.biXPelsPerMeter);
  printf("infoHeader.biYPelsPerMeter: %d\n", infoHeader.biYPelsPerMeter);
  printf("infoHeader.biClrUsed : %d\n", infoHeader.biClrUsed );
  printf("infoHeader.biClrImportant: %d\n", infoHeader.biClrImportant);

  *source_width = (uint32_t)infoHeader.biWidth;
  *source_height = (uint32_t)infoHeader.biHeight;

  // Установка указателя файла на начало данных пикселей
  fseek(source_file_bmp, fileHeader.bfOffBits, SEEK_SET);

  uint32_t width = infoHeader.biWidth;
  uint32_t height = infoHeader.biHeight;
  uint32_t bytesPerPixel = infoHeader.biBitCount / 8;
  uint32_t rowPadded = (width * bytesPerPixel + 3) & (~3);

  buffer = (unsigned char *)malloc(rowPadded * height);
  size_t count_read = fread(buffer, sizeof(unsigned char), rowPadded * height, source_file_bmp);
  // если прочитали не равное размеру количество байт
  if(infoHeader.biSizeImage != count_read) {
    free(buffer);
    return NULL;
  }

  fclose(source_file_bmp);

  return buffer;
}

int save_bmp(char *file_name,
unsigned char *buffer,
const uint32_t new_width,
const uint32_t new_height)
{
    FILE* target_file_bmp = fopen(file_name, "wb");

    if (!target_file_bmp) {
        printf("Ошибка: Не удалось создать файл %s\n", file_name);
        return -1;
    }

    // Вычисляем размер строки с учетом выравнивания до 4 байт
    unsigned int rowPadded = (new_width * 3 + 3) & (~3);
    uint32_t imageSize = rowPadded * abs(new_height);
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
    infoHeader.biWidth = new_width;
    infoHeader.biHeight = new_height; // Положительная высота означает, что изображение хранится снизу вверх
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;   // 24 бита на пиксель
    infoHeader.biCompression = 0; // BI_RGB (без сжатия)
    infoHeader.biSizeImage = imageSize;
    infoHeader.biXPelsPerMeter = 2834;
    infoHeader.biYPelsPerMeter = 2834;
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

    if (new_width * 3 == rowPadded) {
        //Оптимизированная запись, если паддинг не нужен (ширина кратна 4)
        fwrite(buffer, 1, imageSize, target_file_bmp);
    } else {
        // Если паддинг нужен, записываем построчно
        unsigned char padding[3] = {0, 0, 0};
        int paddingSize = rowPadded - (new_width * 3);

        for (int i = 0; i < abs(new_height); i++) {
            // Записываем строку данных
            fwrite(buffer + i * (new_width * 3), 1, new_width * 3, target_file_bmp);
            // Записываем паддинг
            fwrite(padding, 1, paddingSize, target_file_bmp);
        }
    }

    fclose(target_file_bmp);
    printf("Файл %s успешно сохранен.\n", file_name);

  return 0;
}

unsigned char* crop(unsigned char* srcBuffer,
const uint32_t srcWidth,
const uint32_t srcHeight,
const uint32_t startX,
const uint32_t startY,
const uint32_t cropWidth,
const uint32_t cropHeight)
{
 if (startX < 0 || startY < 0 ||
        startX + cropWidth > srcWidth || startY + cropHeight > srcHeight ||
        cropWidth <= 0 || cropHeight <= 0) {
        printf("Ошибка: Некорректные параметры фрагмента.\n");
        return NULL;
    }

    // Определение шага для исходного и преобразованного буферов
    int srcStride = (srcWidth * 3 + 3) & (~3);
    uint32_t destStride = (cropWidth * 3 + 3) & (~3);

    // Рассчитываем размер изображения
    uint32_t destImageSize = destStride * abs(cropHeight);

    // Выделяем память для нового буфера
    unsigned char* destBuffer = (unsigned char*)malloc(destImageSize);
    if (!destBuffer) {
        printf("Ошибка: Недостаточно памяти для нового буфера.\n");
        return NULL;
    }

    for (int i = 0; i < abs(cropHeight); i++) {
        // Определение смещения строки в исходном и преобразованном буферах
        unsigned int srcRowOffset = ((startY + i) * srcStride) + startX * 3;
        uint32_t destRowOffset = (i * destStride);

        // Копирование данных из исходного в преобразованный буфер
        memcpy(destBuffer + destRowOffset, srcBuffer + srcRowOffset, cropWidth * 3);
    }

    return destBuffer;
}

unsigned char *rotate(unsigned char *srcCropBuffer, const uint32_t cropWidthBuffer, const uint32_t cropHeightBuffer)
{
  PIXEL* pixel_array = (PIXEL*)srcCropBuffer;

  unsigned int new_width = cropHeightBuffer; // Ширина и высота меняются местами
  unsigned int new_height = cropWidthBuffer;
  unsigned int padding_out = (4 - (new_width * sizeof(PIXEL)) % 4) % 4;
  PIXEL *dst_data = malloc((new_width * new_height + new_height * padding_out) * sizeof(PIXEL));

    for (int r = 0; r < cropHeightBuffer; r++) {
        for (int c = 0; c < cropWidthBuffer; c++) {
            // Исходные координаты: (c, r)
            // Новые координаты для поворота на 90 градусов ПРОТИВ часовой стрелки:

            // Новая строка = старая ширина - 1 - старый столбец
            unsigned int new_r = cropWidthBuffer - 1 - c;
            // Новый столбец = старая строка
            unsigned int new_c = r;

            unsigned int src_idx = r * cropWidthBuffer + c;
            unsigned int dst_idx = new_r * new_width + new_c;
            // Обратите внимание: new_width используется здесь как количество столбцов в новой строке

            if (dst_idx >= 0 && dst_idx < new_width * new_height) {
                dst_data[dst_idx] = pixel_array[src_idx];
            }
        }
    }

  unsigned char* result = (unsigned char*)dst_data;

  return result;
}
