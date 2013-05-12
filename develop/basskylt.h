#ifndef basskylt_h
#define basskylt_h

#include <sys/uio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BSRESET         0x100   /* reset the board */
#define BSSETBANK       0x101   /* set active memory page */
#define BSGETBANK       0x102   /* get active memory page */
#define BSSETMODE       0x103   /* set active mode (rom/1-bit ram/4-bit ram) */
#define BSGETMODE       0x104   /* get active mode */
#define BSSETRAW        0x105   /* set raw io */
#define BSGETRAW        0x106   /* get raw io */
#define BSCURADDR       0x107   /* query current memory address */
#define BSVERSION       0x108   /* read card version */

class Basskylt {
private:
  char* bitbuf;
  char* buf1bit[4];
  char* buf4bit;
  int wbank;
  int bs;

public:
  class ImageSizeException {};

  Basskylt();
  ~Basskylt();

  class Bankprop {
  private:
    int bs;
    int bank;
  public:
    int operator=(int bnk);
    operator int();
    void Cache();
    SetDev(int b);
  } Bank;

  class Modeprop {
  private:
    int bs;
    int mode;
  public:
    int operator=(int m);
    operator int();
    void Cache();
    SetDev(int b);
  } Mode;

  void PutImage(Image& img, int bank = -1);

  void Cache();

};

extern Basskylt basskylt;

// Bassylt::Bankprop
inline int Basskylt::Bankprop::operator=(int bnk)
{
  bank = bnk;
#ifndef _BSDEBUG
  ioctl(bs,BSSETBANK,bnk);
#endif
}

inline Basskylt::Bankprop::operator int()
{
  return bank;
}

inline void Basskylt::Bankprop::Cache()
{
#ifndef _BSDEBUG
  ioctl(bs,BSGETBANK,&bank);
#else
  bank = 0;
#endif
}

inline Basskylt::Bankprop::SetDev(int b)
{
  bs = b;
}

// Basskylt::Modeprop
inline int Basskylt::Modeprop::operator=(int m)
{
  mode = m;
#ifndef _BSDEBUG
  ioctl(bs,BSSETMODE,m);
#endif
}

inline Basskylt::Modeprop::operator int()
{
  return mode;
}

inline void Basskylt::Modeprop::Cache()
{
#ifndef _BSDEBUG
  ioctl(bs,BSGETMODE,&mode);
#else
  mode = 0;
#endif
}

inline Basskylt::Modeprop::SetDev(int b)
{
  bs = b;
}

// Basskylt

inline Basskylt::~Basskylt()
{
#ifndef _BSDEBUG
  close(bs);
#endif
  delete bitbuf;
}

inline void Basskylt::Cache()
{
#ifndef _BSDEBUG
  lseek(bs, 0, SEEK_SET);
  read(bs, bitbuf, 2048);
#else
  memset(bitbuf, 0, 2048);
#endif
  Bank.Cache();
  Mode.Cache();
}

#endif // basskylt_h
