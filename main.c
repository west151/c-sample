#include <stdlib.h>

#include "bmp.h"

int main()
{
  char *in_file_name = "960px-Lenna.bmp";
  //char *in_file_name = "960px-Lenna-vert.bmp";  // Ширина: 724, Высота: 960
  char *out_file_name = "croup_lenna.bmp";
  uint32_t crop_x = 11;
  uint32_t crop_y = 11;
  uint32_t crop_w = 652;
  uint32_t crop_h = 343;
  uint32_t srcWidth, srcHeight;

  unsigned char *pixel_data = load_bmp(in_file_name, &srcWidth, &srcHeight);

  if(!pixel_data)
    return -1;

  unsigned char *croppedBuffer = crop(pixel_data, srcWidth, srcHeight, crop_x, crop_y, crop_w, crop_h);

  if(!croppedBuffer) {
    free(pixel_data);
    return -1;
  }

  unsigned char *rotateBuffer = rotate(croppedBuffer, crop_w, crop_h);

  if(!rotateBuffer) {
    free(croppedBuffer);
    free(pixel_data);
    return -1;
  }

  // после поворота меняем местами высоту и ширину
  //int code = save_bmp(out_file_name, rotateBuffer, crop_h, crop_w);
  int code = save_bmp(out_file_name, croppedBuffer, crop_w, crop_h);

  free(pixel_data);
  free(croppedBuffer);
  free(rotateBuffer);

  return code;
}

