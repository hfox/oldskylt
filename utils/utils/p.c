#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int bit, i, x, y;
	unsigned char val;
	unsigned char buf[300];

	if(argc < 2) {
		fprintf(stderr, "usage: sp filename\n");
		exit(1);
	}
	if((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "could not open infile %s\n", argv[1]);
		exit(0);
	}

	fread(buf, 1, 288, fp);
	for(y = 0;y < 24;y++) {
		for(x = 0;x < 12;x++) {
			val = buf[x+y*12];
			for(i = 0;i < 8;i++) {
				bit = 1 << i;
				printf("%c", val & bit ? 'X' : ' ');
			}
		}
		printf("\n");
	}
	fclose(fp);
	return 0;
}

