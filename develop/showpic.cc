#include <stdio.h>

#include "image.h"
#include "importpng.h"
#include "basskylt.h"


int main(int argc, char* argv[])
{
  try {
    if (argc == 2) {
      Image img, w(96,24);
      ImportPNG(argv[1], img);
/*
      for (int x = 0; x < img.Width; x ++) {
	for (int y = 0; y < img.Height; y ++)
	  img(x,y) = 0;
        img(x,x % img.Height) = 1;
      }
*/
      for (int x = 0; x < img.Width - 96; x ++) {
	//int bank = (basskylt.Bank + 1) % 4;
	int bank = basskylt.Bank;
	img.GetSubImage(w,x,0,x+95,23);
	basskylt.PutImage(w, bank);
	//basskylt.Bank = bank;
	fflush(stdout);
      }
    }
    else {
      printf("Usage: showpic <picturename>\n");
    }
  }
  catch (Image::FileNotFound) {
    perror("File not ound\n");
  }
  catch (Image::Format) {
    perror("Invalid file format\n");
  }
  catch (Image::Exception e) {
    perror("Image::Exception\n");
  }
}
