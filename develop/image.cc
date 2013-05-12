#include <sys/uio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "image.h"

inline Image::Image(Image& img) : Width(width), Height(height)
{
  SetSize(img.width, img.height);
  memcpy(buf, img.buf, (img.width*img.height)/(8/bitplanes));
  memcpy(lines, img.lines, img.height);
  bitplanes = img.bitplanes;
}

Image::~Image()
{
  if (buf) {
    delete buf;
    delete lines;
  }
}

void Image::SetSize(int w, int h)
{
  if (buf) {
    delete buf;
    delete lines;
  }

  width = w;
  height = h;
  buf = new char[width*height];
  lines = new (char*)[height];
  for (int r = 0; r < height; r ++)
    lines[r] = buf + r*width;
}

void Image::ConvFrom1Bit(char* bitbuf)
{
  char *pbit, *pbyte, *plast;
  char b;

  plast = bitbuf + (width*height)/8;
  for (pbit = bitbuf, pbyte = buf; pbit < plast; pbit ++) {
    b = *pbit;
    *pbyte++ = b & 0x1;
    *pbyte++ = (b & 0x2) >> 1;
    *pbyte++ = (b & 0x4) >> 2;
    *pbyte++ = (b & 0x8) >> 3;
    *pbyte++ = (b & 0x10) >> 4;
    *pbyte++ = (b & 0x20) >> 5;
    *pbyte++ = (b & 0x40) >> 6;
    *pbyte++ = (b & 0x80) >> 7;
  }
}

void Image::ConvFrom4Bit(char* bitbuf)
{
  char *pbit, *pbyte, *plast;

  plast = bitbuf + (width*height)/2;
  for (pbit = bitbuf, pbyte = buf; pbit < plast; pbit ++) {
    *pbyte++ = *pbit & 0x0F;
    *pbyte++ = (*pbit & 0xF0) >> 4;
  }
}

void Image::GetSubImage(Image& subimg, int x1, int y1, int x2, int y2)
{
  if (!subimg.buf)
    SetSize(x2-x1,y2-y1);

  int x, y;
  for (y = y1; y <= y2; y ++)
    for (x = x1; x <= x2; x ++)
      subimg(x-x1,y-y1) = (*this)(x,y);
}

void Image::InsertImage(Image& img, int xp, int yp)
{
  int x, y;
  for (y = 0; y < img.height; y ++)
    for (x = 0; x < img.width; x ++)
      (*this)(x+xp,y+yp) = img(x,y);
}
