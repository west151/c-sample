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

// Структура для хранения одного пикселя (BGR порядок)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;

#pragma pack(pop)

unsigned char *load_bmp(const char *file_name,
                        uint32_t *out_width,
                        uint32_t *out_height);
int save_bmp(const char *file_name,
             unsigned char *buffer,
             const uint32_t width,
             const uint32_t height);
unsigned char* crop(unsigned char* srcBuffer,
                    const uint32_t srcWidth,
                    const uint32_t srcHeight,
                    const uint32_t startX,
                    const uint32_t startY,
                    const uint32_t cropWidth,
                    const uint32_t cropHeight);
unsigned char *rotate_bmp(unsigned char *src,
                          const uint32_t srcWidth,
                          const uint32_t srcHeight);

#endif // BMP_H
