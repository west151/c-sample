#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BITMAPFILEHEADER fh;
BITMAPINFOHEADER ih;

const uint16_t bytesPerPixel = 3;

uint32_t stride_zeroes(const uint32_t width) {
    return (width * bytesPerPixel + 3) & ~3u;
}

unsigned char *load_bmp(const char *file_name,
                        uint32_t *out_width,
                        uint32_t *out_height)
{
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        printf("Error: file not found\n");
        return NULL;
    }

    if (fread(&fh, sizeof(fh), 1, f) != 1) {
        printf("Error: read FILEHEADER\n");
        fclose(f);
        return NULL;
    }

    if (fh.bfType != 0x4D42) {
        printf("Error: not BMP\n");
        fclose(f);
        return NULL;
    }

    if (fread(&ih, sizeof(ih), 1, f) != 1) {
        printf("Error: read INFOHEADER\n");
        fclose(f);
        return NULL;
    }

    if (ih.biBitCount != 24 || ih.biCompression != 0) {
        printf("Error: only 24‑bit uncompressed BMP supported\n");
        fclose(f);
        return NULL;
    }

    if (ih.biHeight < 0) {
        printf("Error: top‑down BMP not supported\n");
        fclose(f);
        return NULL;
    }

    uint32_t width  = (uint32_t)ih.biWidth;
    uint32_t height = (uint32_t)ih.biHeight;

    uint32_t stride = stride_zeroes(width);
    uint32_t img_size = stride * height;

    if (fseek(f, (long)fh.bfOffBits, SEEK_SET) != 0) {
        printf("Error: seek to pixels\n");
        fclose(f);
        return NULL;
    }

    unsigned char *buf = (unsigned char *)malloc(img_size);
    if (!buf) {
        printf("Error: not enough memory\n");
        fclose(f);
        return NULL;
    }

    size_t read_cnt = fread(buf, 1, img_size, f);
    if (read_cnt != img_size) {
        printf("Error: read pixels\n");
        free(buf);
        fclose(f);
        return NULL;
    }

    fclose(f);
    *out_width  = width;
    *out_height = height;
    return buf;
}
int save_bmp(const char *file_name,
             unsigned char *buffer,
             const uint32_t width,
             const uint32_t height)
{
    FILE *f = fopen(file_name, "wb");
    if (!f) {
        printf("Error: can't create file %s\n", file_name);
        return -1;
    }

    uint32_t stride  = stride_zeroes(width);
    uint32_t img_size  = stride * height;
    uint32_t file_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + img_size;

    fh.bfSize = file_size;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = (int32_t)width;
    ih.biHeight = (int32_t)height;
    ih.biSizeImage = img_size;

    fwrite(&fh, sizeof(fh), 1, f);
    fwrite(&ih, sizeof(ih), 1, f);

    unsigned char padding[3] = {0, 0, 0};
    uint32_t pad_size = stride - width * 3;

    for (uint32_t y = 0; y < height; ++y) {
        uint32_t row_off = y * stride;
        fwrite(buffer + row_off, 1, width * 3, f);
        if (pad_size)
            fwrite(padding, 1, pad_size, f);
    }

    fclose(f);
    return 0;
}

unsigned char *crop(unsigned char *src,
                    uint32_t srcWidth,
                    uint32_t srcHeight,
                    uint32_t startX,
                    uint32_t startY,
                    uint32_t cropWidth,
                    uint32_t cropHeight)
{
    if (cropWidth == 0 || cropHeight == 0 ||
        startX + cropWidth  > srcWidth ||
        startY + cropHeight > srcHeight) {
        printf("Error: incorrect parameters\n");
        return NULL;
    }

    uint32_t src_stride = stride_zeroes(srcWidth);
    uint32_t dst_stride = stride_zeroes(cropWidth);
    uint32_t dst_size = dst_stride * cropHeight;

    unsigned char *dst = malloc(dst_size);
    if (!dst) {
        printf("Error: not enough memory\n");
        return NULL;
    }
    memset(dst, 0, dst_size);

    for (uint32_t y_logic = 0; y_logic < cropHeight; ++y_logic) {
        uint32_t src_y_logic = startY + y_logic;
        uint32_t src_y_buf = srcHeight - 1 - src_y_logic;
        uint32_t src_off = src_y_buf * src_stride + startX * 3;

        uint32_t dst_y_buf = cropHeight - 1 - y_logic;
        uint32_t dst_off = dst_y_buf * dst_stride;

        memcpy(dst + dst_off, src + src_off, cropWidth * 3);
    }

    return dst;
}

unsigned char *rotate_bmp(unsigned char *src,
                          const uint32_t srcWidth,
                          const uint32_t srcHeight)
{
    uint32_t src_stride = stride_zeroes(srcWidth);

    uint32_t dstWidth  = srcHeight;
    uint32_t dstHeight = srcWidth;
    uint32_t dst_stride = stride_zeroes(dstWidth);
    uint32_t dst_size  = dst_stride * dstHeight;

    unsigned char *dst = malloc(dst_size);
    if (!dst) {
        printf("Error: not enough memory\n");
        return NULL;
    }
    memset(dst, 0, dst_size);

    for (uint32_t y_buf = 0; y_buf < srcHeight; ++y_buf) {
        for (uint32_t x = 0; x < srcWidth; ++x) {

            uint32_t src_off = y_buf * src_stride + x * 3;

            //uint32_t y_top = srcHeight - 1 - y_buf; // UNUSED !!!!!!!!!!!!

            uint32_t new_x = y_buf;
            uint32_t new_y_top = x;

            uint32_t dst_y_buf  = dstHeight - 1 - new_y_top;
            uint32_t dst_off = dst_y_buf * dst_stride + new_x * 3;

            memcpy(dst + dst_off, src + src_off, 3);
        }
    }
    return dst;
}

