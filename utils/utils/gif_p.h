#ifndef GIF_P_H
#define GIF_P_H

typedef struct {
    char sig[6];
    unsigned short screenwidth;
    unsigned short screenheight;
    unsigned char flags;
    unsigned char background;
    unsigned char aspect;
} GIFHEADER;
#define GIFHEADER_SIZE 13

typedef struct {
    unsigned short left;
    unsigned short top;
    unsigned short width;
    unsigned short height;
    unsigned char flags;
} IMAGEBLOCK;
#define IMAGEBLOCK_SIZE 9

typedef struct {
    char blocksize;
    char flags;
    unsigned short delay;
    char transparent_colour;
    char terminator;
} CONTROLBLOCK;
#define CONTROLBLOCK_SIZE 6

typedef struct {
    char blocksize;
    unsigned short left;
    unsigned short top;
    unsigned short gridwidth;
    unsigned short gridheight;
    unsigned char cellwidth;
    unsigned char cellheight;
    unsigned char forecolour;
    unsigned char backcolour;
} PLAINTEXT;
#define PLAINTEXT_SIZE 13

typedef struct {
    unsigned char blocksize;
    unsigned char applstring[8];
    unsigned char authentication[3];
} APPLICATION;
#define APPLICATION_SIZE 12

typedef struct {
    unsigned short width;
    unsigned short height;
    unsigned short bits;
    unsigned short flags;
    unsigned char palette[768];
} GIFINFO;
#define GIFINFO_SIZE 773

#define NO_CODE		-1

int unpackimage(FILE *fp, short bits, GIFINFO *fi, char *buffer);
int skipextension(FILE *fp);

#endif /* GIF_P_H */
