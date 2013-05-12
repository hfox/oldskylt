#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "bs.h"

usage()
{
	fprintf(stderr, "usage: wmem [count] [value]\n");
	exit(0);
}

main(int argc, char *argv[])
{
	int fd, rc;
	char *buf;
	int val, size;

	if(argc < 3)
		usage();

	size = myatoi(argv[1]);
	val = myatoi(argv[2]);

	if((fd = open("/dev/bs0", O_WRONLY)) == -1) {
		perror("open");
		return 0;
	}
	buf = malloc(size);
	memset(buf, (unsigned char)val, size);

	rc = write(fd, buf, size);
	fprintf(stderr, "wrote %d bytes, value = 0x%x\n", rc, val);
	close(fd);
	return 0;
}

