#include <stdlib.h>

#include "bmp.h"

int main()
{
  char *in_file_name = "960px-Lenna.bmp";
  //char *in_file_name = "960px-Lenna-vert.bmp";  // Ширина: 724, Высота: 960
  char *out_file_name = "croup_lenna.bmp";
  int cropX = 30;
  int cropY = 30;
  int cropW = 540;
  int cropH = 440;
  int srcWidth, srcHeight;

  unsigned char *pixel_data = load_bmp(in_file_name, &srcWidth, &srcHeight);
  unsigned char *croppedBuffer = crop(pixel_data, srcWidth, srcHeight, cropX, cropY, cropW, cropH);
  unsigned char *rotateBuffer = rotate(croppedBuffer, cropW, cropH);
  // после поворота меняем местами высоту и ширину
  int code = save_bmp(out_file_name, rotateBuffer, cropH, cropW);

  free(pixel_data);
  free(croppedBuffer);
  free(rotateBuffer);

  return code;
}

