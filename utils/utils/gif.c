#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gif_p.h"

#ifdef sparc
#define ENDIAN_FIX(a) ((a>>8)+((a&255)<<8))
#else
#define ENDIAN_FIX(a) (a)
#endif

/* 
 * loadgif() - load and unpack a .GIF-file
 *
 * If the loading succeeds, loadgif() returns a pointer to the
 * memory block where the resulting raw image is stored.
 * The GIFINFO struct will be filled out with information about the image.
 * If loading the image fails, loadgif() returns NULL.
 */

char *loadgif(FILE *fp, GIFINFO *fi)
{
    GIFHEADER gh;
    IMAGEBLOCK iblk;
    short b, c;
    char *buffer = NULL;

    /* make sure it's a GIF file */
    if(fread((char *)&gh,1, GIFHEADER_SIZE,fp) != GIFHEADER_SIZE ||
	memcmp(gh.sig, "GIF", 3)) 
	return NULL;

    /* get screen dimensions */
    fi->width = gh.screenwidth;
    fi->height = gh.screenheight;
    fi->bits = (gh.flags & 0x0007) + 1;

    /* get colour map if there is one */
    if (gh.flags & 0x80)
    {
	c = 3 * (1 << ((gh.flags & 7) + 1));
	if(fread(fi->palette, 1, c, fp) != c)
	    return NULL;
    }

    /* step through the blocks */
    while((c=fgetc(fp))==',' || c=='!' || c==0)
    {
	/* if it's an image block... */
	if (c == ',')
	{
	    /* get the start of the image block */
	    if(fread(&iblk, 1, IMAGEBLOCK_SIZE, fp) != IMAGEBLOCK_SIZE)
		return NULL;

	    /* get the image dimensions */
	    fi->width = iblk.width;
	    fi->height = iblk.height;
	    /* get the local colour map if there is one */
	    if(iblk.flags & 0x80)
	    {
		b = 3*(1<<((iblk.flags & 0x0007) + 1));
		if(fread(fi->palette,1,b,fp) != c)
		    return NULL;

		fi->bits=(iblk.flags & 0x0007) + 1;
	    }

	    /* get the initial code size */
	    if((c=fgetc(fp))==EOF)
		return NULL;

	    fi->flags=iblk.flags;

	    /* allocate a buffer */

	    fi->width = ENDIAN_FIX(fi->width);
	    fi->height = ENDIAN_FIX(fi->height);

	    if((buffer = malloc(fi->width * fi->height)) == NULL)
		return NULL;

	    /* unpack the image */
	    if((unpackimage(fp, c, fi, buffer)) == -1)
	    {
		free(buffer);
		return NULL;
	    }
	}
	/* otherwise, it's an extension */
	else if(c == '!')
	    skipextension(fp);
    }
    return buffer;
}
/* ---------------------------------------------------------------------- */

/* unpack an LZW compressed image */
int unpackimage(FILE *fp,short bits, GIFINFO *fi, char *buffer)
{
    short bits2;  	/* Bits plus 1 */
    short codesize;       /* Current code size in bits */
    short codesize2;      /* Next codesize */
    short nextcode;       /* Next available table entry */
    short thiscode;       /* Code being expanded */
    short oldtoken;       /* Last symbol decoded */
    short currentcode;    /* Code just read */
    short oldcode;        /* Code read before this one */
    short bitsleft;       /* Number of bits left in *p */
    short blocksize;      /* Bytes in next block */
    short line=0;	    /* next line to write */
    short byte=0;         /* next byte to write */
    short pass=0;	    /* pass number for interlaced pictures */

    unsigned char *p;	         /* Pointer to current byte in read buffer */
    unsigned char *q;            /* Pointer past last byte in read buffer */
    unsigned char b[255];        /* Read buffer
 */
    unsigned char *u;            /* Stack pointer into firstcodestack */
    unsigned char *linebuffer;   /* place to store the current line */

    static unsigned char firstcodestack[4096];  /* Stack for first codes */
    static unsigned char lastcodestack[4096];   /* Statck for previous code */
    static short codestack[4096];        /* Stack for links */

    static short wordmasktable[] = {	0x0000,0x0001,0x0003,0x0007,
					0x000f,0x001f,0x003f,0x007f,
					0x00ff,0x01ff,0x03ff,0x07ff,
					0x0fff,0x1fff,0x3fff,0x7fff
    };

    static short inctable[] = { 8,8,4,2,0 }; /* shorterlace increments */
    static short startable[] = { 0,4,2,1,0 };  /* interlace starts */

    p=q=b;
    bitsleft = 8;

    if ((bits < 2) || (bits > 8))
	return -1;

    bits2 = 1 << bits;
    nextcode = bits2 + 2;
    codesize2 = 1 << (codesize = bits + 1);
    oldcode = oldtoken = NO_CODE;

    if((linebuffer = malloc(fi->width)) == NULL)
	return -1;

    /* loop until something breaks */
    while(1)
    {
	if(bitsleft == 8)
	{
	    if((++p >= q) && (((blocksize = fgetc(fp)) < 1) ||
		    (q=(p=b)+fread(b,1,blocksize,fp))< (b+blocksize)))
	    {
		free(linebuffer);
		return -1;
	    }
	    bitsleft = 0;
	}
	thiscode = *p;
	if ((currentcode=(codesize+bitsleft)) <= 8)
	{
	    *p >>= codesize;
	    bitsleft = currentcode;
	}
	else
	{
	    if(++p >= q && (((blocksize = fgetc(fp)) < 1) ||
		    (q=(p=b)+fread(b,1,blocksize,fp)) < (b+blocksize)))
	    {
		free(linebuffer);
		return -1;
	    }
	    thiscode |= *p << (8 - bitsleft);
	    if(currentcode <= 16)
		*p >>= (bitsleft=currentcode-8);
	    else
	    {
		if(++p >= q && (((blocksize = fgetc(fp)) < 1) ||
			(q=(p=b) + fread(b,1,blocksize,fp)) < (b+blocksize)))
		{
		    free(linebuffer);
		    return -1;
		}
		thiscode |= *p << (16 - bitsleft);
		*p >>= (bitsleft = currentcode - 16);
	    }
	}
	thiscode &= wordmasktable[codesize];
	currentcode = thiscode;

	if(thiscode == (bits2+1))
	    break;	/* found EOI */
	if(thiscode > nextcode)
	{
	    free(linebuffer);
	    return -1;
	}

	if(thiscode == bits2)
	{
	    nextcode = bits2 + 2;
	    codesize2 = 1 << (codesize = (bits + 1));
	    oldtoken = oldcode = NO_CODE;
	    continue;
	}

	u = firstcodestack;

	if(thiscode == nextcode)
	{
	    if(oldcode == NO_CODE)
	    {
		free(linebuffer);
		return -1;
	    }
	    *u++ = oldtoken;
	    thiscode = oldcode;
	}
	
	while (thiscode >= bits2)
	{
	    *u++ = lastcodestack[thiscode];
	    thiscode = codestack[thiscode];
	}

	oldtoken = thiscode;
	while(1)
	{
	    linebuffer[byte++]=thiscode;
	    if(byte >= fi->width)
	    {
		memcpy(&buffer[line * fi->width], linebuffer, fi->width);
		byte=0;

		/* check for interlaced image */
		if(fi->flags & 0x40)
		{
		    line+=inctable[pass];
		    if(line >= fi->height)
			line = startable[++pass];
		} 
		else
		    line++;
	    }

	    if (u <= firstcodestack)
		break;
	    thiscode = *--u;
	} 

	if(nextcode < 4096 && oldcode != NO_CODE)
	{
	    codestack[nextcode] = oldcode;
	    lastcodestack[nextcode] = oldtoken;
	    if(++nextcode >= codesize2 && codesize < 12)
		codesize2 = 1 << ++codesize;
	}
	oldcode = currentcode;
    }

    free(linebuffer);
    return 0;
}

int skipextension(FILE *fp)
{
    PLAINTEXT pt;
    CONTROLBLOCK cb;
    APPLICATION ap;
    short n,i;

    switch(fgetc(fp))
    {
	case 0x0001:		/* plain text descriptor */
	    if(fread((char *)&pt, 1, PLAINTEXT_SIZE,fp) == PLAINTEXT_SIZE)
	    {
		do
		{
		    if((n=fgetc(fp)) != EOF)
		    {
			for(i = 0;i < n;i++)
			    fgetc(fp);
		    }
		} while(n > 0 && n != EOF);
	    }
	    else
		fprintf(stderr, "Error reading plain text block");
	    break;

	case 0x00f9:		/* graphic control block */
	    if(fread((char *)&cb,1, CONTROLBLOCK_SIZE,fp) != CONTROLBLOCK_SIZE)
		fprintf(stderr, "Error reading control block");
	    break;

	case 0x00fe:		/* comment extension */
	    do
	    {
		if((n=fgetc(fp)) != EOF)
		{
		    for(i = 0;i < n;i++)
			fgetc(fp);
		}
	    } while(n > 0 && n != EOF);
	    break;

	case 0x00ff:		/* application extension */
	    if(fread((char *)&ap,1, APPLICATION_SIZE, fp) == APPLICATION_SIZE)
	    {
		do
		{
		    if((n=fgetc(fp)) != EOF)
		    {
			for(i=0;i<n;++i)
			    fgetc(fp);
		    }
		} while(n > 0 && n != EOF);
	    } 
	    else
		fprintf(stderr, "Error reading application block");
	    break;
	    
	default:		/* something else */
	    n = fgetc(fp);
	    for(i = 0;i < n;i++)
		fgetc(fp);
	    break;
    }
    return 0;
}
