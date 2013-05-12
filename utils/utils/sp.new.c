/*
 * Show Picture, sp.c
 * 
 * Written ~1998 by <datateknolog>
 * 
 * Modified 2011-05-16 by Valentin Assmundsson (DRust'011)
 *  to hack support for 4-bit-colour.
 * 
 */


#include <stdio.h>


#define COLS		96		/* LEDs */
#define ROWS		24		/* LEDs */
#define ROWSIZ		COLS/8		/* bytes */
#define SCREENSIZ	ROWSIZ*ROWS	/* bytes */

static const char * const modechars = " .-nox;%*O0X8#@MERrad";
static const char mclen = 16;

int main(int argc, char *argv[])
{
	FILE *fp;
	char mode = 0;
	int size; 
	int bit, i, j, x, y;
	unsigned char *valp,  val;
	unsigned char buf[2048];

	if(argc < 2) {
		fprintf(stderr, "usage: sp filename\n");
		exit(1);
	}
	if((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "could not open infile %s\n", argv[1]);
		exit(0);
	}
	
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	printf("\nsize: %d\n", size);
	if(size==2048){
		mode = 4;
	} else {
		mode = 1;
	}
	fseek(fp,0,SEEK_SET);
	
	
	fread(buf, 1, 512*mode, fp);
	for(y = 0;y < 24;y++) {
		for(x = 0;x < 12;x++) {
			valp = &buf[x+y*12];
			bit = 1; /* There has to be a bit to the right to loop with */
			for(i = 0;i < 8;i++) {
				val = 0;
				for (j = 0;j < mode; j++)
				{
					val |= (!!(buf[x + y*12 + 512*j] &
bit)) << j;
				}
				if(mode == 1)
					printf("%c",
modechars[val*15]);
				else
					printf("%c",
modechars[val]);
					
/*				printf("%d,",val);*/
				bit = bit << 1;
			}
		}
		printf("\ny:%02d", y);
	}
	fclose(fp);
	return 0;
}

