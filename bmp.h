#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

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

// struct BITMAPINFOHEADER
// {
//     DWORD Size; // Число байтов необходимое для структуры = 40
//     DWORD Width; // Ширина точечного рисунка в пикселях
//     DWORD Height; // Высота точечного рисунка в пикселях
//     WORD Planes; // Число плоскостей целевого устройства = 1
//     WORD BitCount; // Глубина цвета, число бит на точку = 0,1,4,8,16,24,32
//     DWORD Compression; // Тип сжатия = 0 для несжатого изображения
//     DWORD SizeImage; // Размер изображения в байтах BitCount*Height*Width
//     DWORD XPelsPerMeter; // Разрешающая способность по горизонтали
//     DWORD YPelsPerMeter; // Разрешающая способность по вертикали
//     DWORD ColorUsed; // Число индексов используемых цветов. Если все цвета = 0
//     DWORD ColorImportant; // Число необходимых цветов = 0
// };

// Структура для хранения одного пикселя (BGR порядок)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

#pragma pack(pop)

unsigned char* load_bmp(char *file_name, int *sourceWidth, int *sourceHeight);
int save_bmp(char *file_name, unsigned char *buffer, int width, int height);
unsigned char* crop(unsigned char* srcBuffer, int srcWidth, int srcHeight, int startX, int startY, int cropWidth, int cropHeight);
unsigned char* rotate(unsigned char* srcCropBuffer, int cropWidthBuffer, int cropHeightBuffer);
unsigned char* read_line(unsigned char* buffer, int width_buffer);

#endif // BMP_H
