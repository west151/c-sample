#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

typedef struct {
    unsigned char	b1,b2;			//Символы BM (смение 0, длина 2)
    unsigned long	bfSize;			//Размер файла в байтах	(Смещение 2, длина 4)
    unsigned short	bfReserved1;	        //Бесполезно (Смещение 6, длина 2)
    unsigned short	bfReserved2;	        //Бесполезно (Смещение 8, длина 2)
    unsigned long	bfOffBits;		//Смещение до самого изображения (Смещение 10, длина 4)
} bmpheader;

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
} Pixel;

#pragma pack(pop)

#endif // BITMAP_H
