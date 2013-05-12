#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "bs.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define BITSPERPIC 2304
#define BYTESPERPIC 288


char *skyltaddr;

void MakeUnsortedArray(int* pArray, int nSize)
{
  int i, t, p;

  for (i = 0; i < nSize; i ++)
    pArray[i] = i;

  for (i = 0; i < nSize; i ++) {
    p = rand() % nSize;
    t = pArray[i];
    pArray[i] = pArray[p];
    pArray[p] = t;
  }
}

void NextImage(const char* pFilename, int nSpeed)
{
  int pArray[BITSPERPIC];
  char pBuf[BYTESPERPIC];
  int nBS, nFile;
  unsigned char nBank, nMode;
  int i;
  char b, mask;
  clock_t time;

  if ((nBS = open("/dev/bs0", O_RDWR)) == -1) {
    perror("Kan inte komma åt basskylten\n");
    return;
  }

  if((skyltaddr = (char *) mmap(0, 2048, (PROT_READ|PROT_WRITE), 
                            MAP_SHARED, nBS, 0)) == -1) {
    perror("mmap");
    exit(-1);
  }


  if ((nFile = open(pFilename, O_RDONLY)) == -1) {
    perror("Kan inte öppna bilden\n");
    return;
  }
  nMode = 1;
  ioctl(nBS, BSSETMODE, &nMode); 
  ioctl(nBS, BSGETMODE, &nMode);
  if (nMode != 1) {
    perror("Kan bara köras med 1 bitplan\n");
    return;
  }

  ioctl(nBS, BSGETBANK, &nBank);

  MakeUnsortedArray(pArray, BITSPERPIC);

  if (read(nFile, pBuf, BYTESPERPIC) != BYTESPERPIC) {
    perror("Bildens storlek stämmer inte med skyltens\n");
    return;
  }

  for (i = 0; i < BITSPERPIC; i ++) {
    time = clock();
    b = *(skyltaddr+nBank*512+(pArray[i]/8));
    mask = 1 << (pArray[i]%8);
    b = (b & ~mask) | (pBuf[pArray[i]/8] & mask);
    *(skyltaddr+nBank*512+(pArray[i]/8)) = b;
    //if(*(skyltaddr+nBank*512+(pArray[i]/8)) != b)
    //  *(skyltaddr+nBank*512+(pArray[i]/8)) = b;
    if((i%30) == 0) {
      usleep(10);
    }
    //usleep(nSpeed*1000/BITSPERPIC - (clock() - time));
  }
  if(munmap((void *)skyltaddr, 2048) == -1) {
    perror("munmap");
  }
  nBank = (nBank + 1) % 4;
  lseek(nBS, nBank*512, SEEK_SET);
  write(nBS, pBuf, 288);
  ioctl(nBS, BSSETBANK, &nBank);
 
  close(nFile);
  close(nBS);
}

void DisplayUsage() {
  perror("random [-s<tid>] <bildfil>\n");
  perror("Tid anges i millisekunder.\n");
}

int main(int argc, char* argv[])
{
  int speed;
  if (argc == 3) {
    if (sscanf(argv[1],"-s%d",&speed) != 1) {
      DisplayUsage();
      return 0;
    }
    NextImage(argv[2], speed);
  }
  else if(argc == 2) {
    speed = 1000;
    NextImage(argv[1], speed);
  }
  else
    DisplayUsage();

  return 0;
}
