
#include "image.h"
#include "basskylt.h"
#include "importpng.h"
#include <stdio.h>
#define CHARWIDTH 6
#define CHARHEIGHT 11

void ConvertText(char* pText)
{
  char* p;
  char last = '~' - 0x20;

  for (p = pText; *p; p ++) {
    if (*p < 20)
      *p = 0;
    else if (*p < '~')
      *p -= 0x20;
    else {
      switch (*p) {
      case 'Å':
        *p = last + 1;
        break;
      case 'Ä':
        *p = last + 2;
        break;
      case 'Ö':
        *p = last + 3;
        break;
      case 'å':
        *p = last + 4;
        break;
      case 'ä':
        *p = last + 5;
        break;
      case 'ö':
        *p = last + 6;
        break;
      default:
        *p = 0;
      }
    }
  }
}

void MakeTextImg(Image& img, char* text)
{
  Image font;
  ImportPNG("font1.pnm",font);
  Image fchar(CHARWIDTH,CHARHEIGHT);
  int l = strlen(text);

  img.SetSize(l*CHARWIDTH + 2*96,24);
  ConvertText(text);
  for (int i = 0; i < l; i ++) {
    font.GetSubImage(fchar,text[i]*CHARWIDTH,0,
		     (text[i]+1)*CHARWIDTH-1,CHARHEIGHT);
    img.InsertImage(fchar, 96 + i*CHARWIDTH,5);
  }
}

int main(int argc, char* argv[])
{
/*  if (argc == 2) {
    Image img;
    MakeTextImg(img,argv[1]);
    for (int x = 0; x < img.Width - 96; x ++) {
      int bank = (basskylt.Bank + 1) % 4;
      img.GetSubImage(w,x,0,x+95,23);
      basskylt.PutImage(w, bank);
      basskylt.Bank = bank;
    }
  }
  return 0;
*/
  Image img;
  ImportPNG("pic1.pnm",img);
  Image i2(img.Width,24);
  i2.InsertImage(img,0,0);
  Image i3(96,24);
  for(int x = 0; x < img.Width - 96; x ++) {
    i2.GetSubImage(i3,0,0,95,23);
    basskylt.PutImage(i3,basskylt.Bank);
  }
  return 0;
}

