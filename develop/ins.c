#include <stdio.h>
#include <stdlib.h>

void usage()
{
	fprintf(stderr, "usage: ins {-d|-i}\n");
	exit(1);
}

main(int argc, char *argv[])
{
	if(argc < 2)
		usage();
	if(argv[1][0] != '-')
		usage();

	switch(argv[1][1]) {
		case 'd':
			system("/sbin/rmmod bs");
			break;
		case 'i':
			system("/sbin/insmod bs.o");
			break;
		default:
			usage();
	}
	return 0;
}

