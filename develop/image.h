#ifndef IMAGE_H
#define IMAGE_H

class Image {
 protected:
  char* buf;
  char** lines;
  int width, height;
  int bitplanes;

 public:
  class Exception { };
  class FileNotFound : public Exception {};
  class Format : public Exception {};

  Image();
  Image(Image&);
  Image(int w, int h);
  ~Image();

  void SetSize(int w, int h);

  const int& Width;
  const int& Height;

  void ConvFrom1Bit(char* bitbuf);
  void ConvFrom4Bit(char* bitbuf);

  void GetSubImage(Image& img, int x1, int y1, int x2, int y2);
  void InsertImage(Image& img, int x, int y);
  char& operator()(int x, int y);
  char* GetBuf();
};

inline Image::Image() : Width(width), Height(height)
{
  buf = 0;
}

inline char& Image::operator()(int x, int y)
{
  return *(lines[y]+x);
}

inline Image::Image(int w, int h) : Width(width), Height(height)
{
  buf = 0;
  lines = 0;
  SetSize(w,h);
}

inline char* Image::GetBuf()
{
  return buf;
}

#endif
