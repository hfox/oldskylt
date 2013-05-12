#include <stdlib.h>
#include <stdio.h>
#include "bs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


main(int argc, char **argv)
{
int fd;
unsigned char apa;
char a = 0;

char b = 1;
char c = 2;
char d = 3;
	if((fd = open("/dev/bs0", O_RDONLY)) == -1) {
		perror("open");
		exit(-1);
	}
	
	ioctl(fd, BSSETBANK, &a);
	sleep(2);
	ioctl(fd, BSSETBANK, &c);	
	sleep(2);
	ioctl(fd, BSSETBANK, &d);
	sleep(2);
	ioctl(fd, BSSETBANK, &b);
	ioctl(fd, BSGETBANK, &apa);
	printf("bank %02x\n",(unsigned char)apa);
	sleep(2);
	ioctl(fd, BSSETMODE, &a);
	sleep(10);
	ioctl(fd, BSSETMODE, &d);
	ioctl(fd, BSGETMODE, &apa);
	printf("mode %02x\n", (unsigned char)apa);
		sleep(3);
	ioctl(fd, BSSETMODE, &b);
	ioctl(fd, BSGETRAW, &apa);
	printf("raw %02x\n", (unsigned char)apa);
		
	close(fd);
}