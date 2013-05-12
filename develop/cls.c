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
#include <string.h>

#define BITSPERPIC 2304
#define BYTESPERPIC 288
#define BYTESPERLINE 12
#define LINES 24

char *skyltaddr;

void NextImage(const char* pFilename, int nSpeed)
{
  char pBuf[BYTESPERPIC*2];
  int nBS, nFile;
  unsigned char nBank, nMode;
  int i;
  clock_t time;

  if ((nFile = open(pFilename, O_RDONLY)) == -1) {
    perror("Kan inte öppna bilden\n");
    return;
  }

  if (read(nFile, pBuf+BYTESPERPIC, BYTESPERPIC) != BYTESPERPIC) {
    perror("Bildens storlek stämmer inte med skyltens\n");
    return;
  }

  close(nFile);

  if ((nBS = open("/dev/bs0", O_RDWR)) == -1) {
    perror("Kan inte komma åt basskylten\n");
    return;
  }

  if((skyltaddr = (char *) mmap(0, 2048, (PROT_READ|PROT_WRITE), 
                            MAP_SHARED, nBS, 0)) == -1) {
    perror("mmap");
    exit(-1);
  }

  nMode = 1;
  ioctl(nBS, BSSETMODE, &nMode); 

  ioctl(nBS, BSGETBANK, &nBank);

  memcpy(pBuf, skyltaddr + nBank*512, BYTESPERPIC);

  for (i = 0; i <= BYTESPERPIC; i += BYTESPERLINE) {
    time = clock();
    nBank = (nBank + 1) % 4;
    memcpy(skyltaddr + nBank*512, pBuf + i, BYTESPERPIC);
    usleep((1000*nSpeed)/LINES - (clock() - time));
    ioctl(nBS, BSSETBANK, &nBank);
  }

  if(munmap((void *)skyltaddr, 2048) == -1) {
    perror("munmap");
  }
 
  close(nBS);
}

void DisplayUsage() {
  perror("vscroll [-s<tid>] <sif-fil>\n");
  perror("tid anges i millisekunder.\n");
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
