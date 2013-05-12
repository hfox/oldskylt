#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gif_p.h"

char *loadgif(FILE *fp, GIFINFO *fi);
void writefile(FILE *fp, char *img);
void error(char *s,...);

void usage()
{
	fprintf(stderr,"usage: %s [-o output-file] [-O origin] infile\n", argv[0]);
	exit(0);
}

int parse_origin(char *s, int *rx, int *ry)
{
	*rx = *ry = 0;
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fp, *ofp;
	int i;
	int o_spec = 0;
	char *infile = NULL, *ofile = NULL, *o_str;
	int x = 0, y = 0;
	GIFINFO hdr;
	char *img, *ptr;

	if(argc < 2)
	    usage();

	for(i = 1;i < argc;i++) {
	    if(argv[i][0] != '-') {
		if(infile != NULL)
		    error("multiple infiles specified");

		infile = argv[i];
		continue;
	    }
	    switch(argv[i][1]) {
		case 'O':
		    if(argv[i][2] != '\0') {
			o_str = &argv[i][2];
		    } else {
			if(i >= argc - 1)
			    usage();
			i++;
			o_str = argv[i];
		    }
		    o_spec = 1;
		    break;

		case 'o':
		    if(argv[i][2] != '\0') {
			ofile = &argv[i][2];
		    } else {
			if(i >= argc - 1)
			    usage();
			i++;
			ofile = argv[i];
		    }
		    break;

		default:
		    usage();
	    }
	}
	if(infile == NULL)
	    error("no input file\n");

	if(o_spec) {
	    if((parse_origin(o_str, &x, &y)) == -1)
		error("malformed origin specifier");
	}
	if((fp = fopen(infile, "r")) == NULL) {
	    error("could not open infile %s\n", infile);
	}
	if((img = loadgif(fp, &hdr)) == NULL)
	    error("failed reading GIF-file. File corrupt?");

	if(ofile == NULL) {
	    if((ofile = malloc((strlen(infile) + 5) * sizeof(char))) == NULL)
		error("malloc failed");

	    strcpy(ofile, infile);
	    if((ptr = strrchr(ofile, '.')) != NULL)
		*ptr = '\0';
	    strcat(ofile, ".sif");
	}
	if((ofp = fopen(ofile, "w")) == NULL)
	 //   error("failed opening outfile: %s", strerror(errno));
	    error("failed opening outfile: xxx");
	writefile(ofp, img);
	fclose(ofp);
	fclose(fp);
	return 0;
}

void writefile(FILE *fp, char *img)
{
	int i, j;
	char *planes[4];
	int offset = 0;
	unsigned char pixels;
	int val, bit;

	planes[0] = malloc(512);

	for(i = 0;i < 24;i++) {
	    bit = 0;
	    pixels = 0;
	    for(j = 0;j < 96;j++) {
		val = img[i*96+j];
		if(val) {
		    printf("x");
		    pixels |= (1 << (7-bit));
		} else {
		    printf(" ");
		}
		bit++;
		if(bit > 7) {
		    planes[0][offset] = pixels;
		    bit = 0;
		    offset++;
		    pixels = 0;
		}
	    }
	    printf("\n");
	}
	fwrite(planes[0], 1, 512, fp);
}

void error(char *s, ...)
{
	va_list ap;

	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);

	exit(1);
}
