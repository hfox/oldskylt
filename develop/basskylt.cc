#include "image.h"
#include "basskylt.h"

Basskylt basskylt;

Basskylt::Basskylt()
{
#ifndef _BSDEBUG
  bs = open("/dev/bs0",O_RDWR);
#endif
  Bank.SetDev(bs);
  Mode.SetDev(bs);
  bitbuf = new char[2048];
  buf1bit[0] = bitbuf;
  buf1bit[1] = bitbuf + 512;
  buf1bit[2] = bitbuf + 1024;
  buf1bit[3] = bitbuf + 1536;
  buf4bit = bitbuf;
  Cache();
}

void Basskylt::PutImage(Image& img, int bank)
{
  char* b;
  if (Mode == 1) {
    if (bank == -1)
      bank = Bank;
    if (img.Width != 96 || img.Height != 24)
      throw ImageSizeException();
    b = buf1bit[bank];
    for (int y = 0; y < 24; y ++)
      for (int x = 0; x < 96; x+= 8)
	*b++ =
	  (img(x+7,y) << 7) | (img(x+6,y) << 6) |
	  (img(x+5,y) << 5) | (img(x+4,y) << 4) |
	  (img(x+3,y) << 3) | (img(x+2,y) << 2) |
	  (img(x+1,y) << 1) | img(x,y);

#ifndef _BSDEBUG
    lseek(bs, bank*512, SEEK_SET);
    write(bs, buf1bit[bank], 288);
#endif
  }
}
