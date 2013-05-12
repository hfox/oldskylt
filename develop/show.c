#include <stdlib.h>
#include <stdio.h>
#include "bs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#ifndef NO_MMAP
#define MMAP
#endif

#define BS_MEM_SIZE 2048

void help();

void usage(char *prgname)
{
  fprintf(stderr, "usage: %s delay #loops file1 ...\n", prgname);
  help();
}

void help()
{
#ifdef MMAP
  fprintf(stderr, "Using mmap()\n");
#else
  fprintf(stderr, "Not using mmap()\n");
#endif
}	

void main(int argc, char **argv)
{
  int n, x;
  int bs;
  int delay;
  int nloops;
  unsigned char bank;
  unsigned char mode;
  unsigned char buf[288];
  int e = 0;
  char *skyltaddr;

  if(argc < 4) {
    usage(argv[0]);
    exit(-1);
  }
  delay = atoi(argv[1]);
  nloops = atoi(argv[2]);
  if(nloops == 0)
    e = 1;
  if(delay < 1)
    delay = 1;
  if((bs=open("/dev/bs0", O_RDWR)) == -1) {
    perror("open");
    exit(-1);
  }

#ifdef MMAP
  if((skyltaddr = (char *) mmap(0, BS_MEM_SIZE, (PROT_READ|PROT_WRITE), 
                            MAP_SHARED, bs, 0)) == -1) {
                                perror("mmap");
  }
#endif

  ioctl(bs, BSGETMODE, &mode);
  if(mode != 1) {
    mode = 1;
    ioctl(bs, BSSETMODE, &mode);
  }
  ioctl(bs, BSGETBANK, &bank);
  
  for(x=0; (x<nloops) || (e == 1); x++) {
    for(n = 0; n < argc-3; n++) {
      int fd;
      if((fd = open(argv[n+3], O_RDONLY)) == -1) {
	fprintf(stderr,"file: %s\n", argv[n+3]);
	perror("");
      } else {
	if(read(fd, buf, 288) != 288)
	  fprintf(stderr, "too small picture\n");
	bank = (bank+1)%4;
#ifndef MMAP
	lseek(bs, bank*512, SEEK_SET);
	write(bs, buf, 288);
#else
        memcpy(skyltaddr+bank*512, buf, 288);
#endif
	ioctl(bs, BSSETBANK, &bank);
	close(fd);
	usleep(10000*delay);
      }
    }
	
  }
#ifdef MMAP
  if(munmap((void *)skyltaddr, BS_MEM_SIZE) == -1) {
    perror("munmap");
  }
#endif
  close(bs);
}
