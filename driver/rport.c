#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm/io.h>

#define SZ_16 1
#define SZ_8 2

void usage()
{
	fprintf(stderr, "usage: rport [-b|-w] <-p portnum>\n");
	fprintf(stderr, "       -b: use 8 bit I/O (default)\n"
			"       -w: use 16 bits\n");
	exit(0);
}

int myatoi(char *s)
{
	int value = 0;
	int mult = 10;
	if(*s == '0') {
		s++;
		if(*s == 'x') {
			s++;
			mult = 16;
		} else {
			mult = 8;
		}
	}
	while(*s) {
		if(isalpha(*s)) {
			value = value * mult + (10 + toupper(*s) - 'A');
		} else {
			value = value * mult + *s - '0';
		}
		s++;
	}
	return value;
}

int main(int argc, char *argv[])
{
	int value, port, size;
	int i;
	size = SZ_8;

	if(argc < 2)
		usage();
	for(i = 1;i < argc;i++) {
		if(argv[i][0] != '-')
			usage();
		switch(argv[i][1]) {
			case 'p':
				if(i > argc - 1)
					usage();
				port = myatoi(argv[++i]);
				break;
			case 'w':
				size = SZ_16;
				break;
			case 'b':
				size = SZ_8;
				break;
			default:
				usage();
		}
	}
	if((iopl(3)) == -1) {
		perror("failed setting I/O privilege level");
		exit(0);
	}
	if(size == SZ_8) {
		value = inb(port);
	} else {
		value = inw(port);
	}
	printf("read value 0x%x from port 0x%x\n", value, port);
	return 0;
}

