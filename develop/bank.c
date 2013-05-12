#include <stdlib.h>
#include <stdio.h>
#include "bs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage(char *prg)
{
	fprintf(stderr, "Usage: %s { -s {0|1|2|3} | -r }\n", prg);
	return;
}

main(int argc, char **argv)
{
int fd;
unsigned char bank;
unsigned int cmd;

switch(argc) {
	case 2:
	if(strcmp(argv[1], "-r") == 0) {
		cmd = BSGETBANK;
	} else {
	usage(argv[0]);
	exit(-1);
	}
	break;
	case 3:
	if(strcmp(argv[1], "-s") == 0) {
		cmd = BSSETBANK;		
		bank = (unsigned char)atoi(argv[2]);
		if(bank > 3) {
			usage(argv[0]);
			exit(-1);
		}
	} else {
		usage(argv[0]);
		exit(-1);
	}
	break;
	default:
	usage(argv[0]);
	exit(-1);
	
}

	if((fd = open("/dev/bs0", O_RDONLY)) == -1) {
		perror("open");
		exit(-1);
	}
	
	ioctl(fd, cmd, &bank);
	close(fd);
	if(cmd == BSGETBANK)
		fprintf(stdout, "%d\n", (int)bank);
	exit(0);
}