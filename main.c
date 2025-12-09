#include <stdlib.h>

#include "bmp.h"

int main()
{
  char *in_file_name = "960px-Lenna.bmp";
  //char *in_file_name = "960px-Lenna-vert.bmp";  // Ширина: 724, Высота: 960
  char *out_file_name = "croup_lenna.bmp";
  int cropX = 100;
  int cropY = 133;
  int cropW = 652;
  int cropH = 344;
  unsigned int srcWidth, srcHeight;

  unsigned char *pixel_data = load_bmp(in_file_name, &srcWidth, &srcHeight);

  if(!pixel_data)
    return -1;

  unsigned char *croppedBuffer = crop(pixel_data, srcWidth, srcHeight, cropX, cropY, cropW, cropH);

  if(!croppedBuffer) {
    free(pixel_data);
    return -1;
  }

  unsigned char *rotateBuffer = rotate(croppedBuffer, cropW, cropH);

  if(!rotateBuffer) {
    free(croppedBuffer);
    free(pixel_data);
    return -1;
  }

  // после поворота меняем местами высоту и ширину
  int code = save_bmp(out_file_name, rotateBuffer, cropH, cropW);
  //int code = save_bmp(out_file_name, croppedBuffer, cropW, cropH);

  free(pixel_data);
  free(croppedBuffer);
  free(rotateBuffer);

  return code;
}

