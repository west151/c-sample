#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t bfType;       // Сигнатура "BM"
    uint32_t bfSize;       // Размер файла
    uint16_t bfReserved1;  // Зарезервировано (0)
    uint16_t bfReserved2;  // Зарезервировано (0)
    uint32_t bfOffBits;    // Смещение до данных изображения
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;           // Размер структуры
    int32_t  biWidth;          // Ширина изображения
    int32_t  biHeight;         // Высота изображения
    uint16_t biPlanes;         // Всегда 1
    uint16_t biBitCount;       // Глубина цвета (например, 24)
    uint32_t biCompression;    // Тип сжатия
    uint32_t biSizeImage;      // Размер данных изображения
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

// Структура для хранения одного пикселя (BGR порядок)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;

unsigned char* load_bmp(char *file_name, int *sourceWidth, int *sourceHeight);
int save_bmp(char *file_name, unsigned char *buffer, int width, int height);
unsigned char* crop(unsigned char* srcBuffer, int srcWidth, int srcHeight, int startX, int startY, int cropWidth, int cropHeight);
unsigned char* rotate(unsigned char* srcCropBuffer, int cropWidthBuffer, int cropHeightBuffer);
// void rotate_pixels(PIXEL *src_data, PIXEL *dst_data, int rows, int cols);



// void flip_buffer_horizontal(unsigned char* buffer, int width, int height);
// void flip_buffer_vertical(unsigned char* buffer, int width, int height);

#endif // BMP_H
