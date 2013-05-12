#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "bs.h"

int main(void)
{
	int fd, rc;
	char *buf;
	if((fd = open("/dev/bs0", O_RDONLY)) == -1) {
		perror("open");
		return 0;
	}
	if((ioctl(fd, 0, NULL)) == -1) {
		perror("ioctl1");
 	}
	if((ioctl(fd, BSRESET, NULL)) == -1) {
		perror("ioctl");
	}
	buf = malloc(2048);
	rc = read(fd, buf, 2048);
	fprintf(stderr, "read %d bytes\n", rc);
	write(1, buf, rc);
	close(fd);

	return 0;
}

