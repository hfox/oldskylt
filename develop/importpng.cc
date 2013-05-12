#include "importpng.h"
#include <stdio.h>

int ImportPNG(const char* filename, Image& img)
{
  FILE* fp;
  char buf[300];

  if ((fp = fopen(filename, "r")) == NULL) {
    return 0;
  }

  fgets(buf, 300, fp);
  fgets(buf, 300, fp);
  int width, height;
  fscanf(fp, "%d %d", &width, &height);
  fgets(buf, 300, fp);

  img.SetSize(width, height);
  
  for (int y = 0; y < img.Height; y ++) {
    for (int x = 0; x < img.Width; x ++) {
      img(x,y) = getc(fp) + getc(fp) + getc(fp) > 100 ? 1 : 0;
    }
  }

  fclose(fp);
}
