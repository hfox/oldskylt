#include <stdio.h>

int main(int argc, char *argv[])
{

FILE *infil,*utfil;
char image[512];
int i;
infil=fopen(argv[1],"r");
fread(image,1,512,infil);
fclose(infil);
for(i=0;i<512;i++)
 image[i]=image[i]^0xff;
utfil=fopen(argv[2],"w");
fwrite(image,1,512,utfil);
fclose(utfil);

}
