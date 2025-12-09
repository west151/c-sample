#include <stdlib.h>

#include "bmp.h"

int main()
{
  char *in_file_name = "960px-Lenna.bmp";
  //char *in_file_name = "960px-Lenna-vert.bmp";  // Ширина: 724, Высота: 960
  char *out_file_name = "croup_lenna.bmp";
  int cropX = 30;
  int cropY = 33;
  int cropW = 552;
  int cropH = 244;
  int srcWidth, srcHeight;

  unsigned char *pixel_data = load_bmp(in_file_name, &srcWidth, &srcHeight);

  if(!pixel_data)
    return -1;

  unsigned char *croppedBuffer = crop(pixel_data, srcWidth, srcHeight, cropX, cropY, cropW, cropH);

  if(!croppedBuffer)
    return -1;

  unsigned char *rotateBuffer = rotate(croppedBuffer, cropW, cropH);

  if(!rotateBuffer)
    return -1;

  // после поворота меняем местами высоту и ширину
  int code = save_bmp(out_file_name, rotateBuffer, cropH, cropW);
  //int code = save_bmp(out_file_name, croppedBuffer, cropW, cropH);

  free(pixel_data);
  free(croppedBuffer);
  free(rotateBuffer);

  return code;
}

