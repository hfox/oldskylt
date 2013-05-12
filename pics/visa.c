#include<stdio.h>
#include<sys/io.h>
#include<fcntl.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
int rc,fd;
FILE *filen;
char image[512];
int i;



if(argc!=5) 
  printf("usage:visa filnamn1 filnamn2 filnamn3 filnamn4");
else
  {

  if((fd=open("/dev/bs0",O_RDWR))==-1)
        return -1; 
 
 filen=fopen(argv[1],"r");
 fread(image,1,512,filen); 
 lseek(fd,0,SEEK_SET);
 write(fd,image,512);/*skriv en gang till kortet*/
 lseek(fd,0,SEEK_SET);
 write(fd,image,512);/*och en gang till foer att vara saker*/
 fclose(filen);

 

 filen=fopen(argv[2],"r");
 fread(image,1,512,filen);
 lseek(fd,512,SEEK_SET);
 write(fd,image,512);
 lseek(fd,512,SEEK_SET);
 write(fd,image,512);
 fclose(filen);
 
 filen=fopen(argv[3],"r");
 fread(image,1,512,filen);
 lseek(fd,1024,SEEK_SET);
 write(fd,image,512);
 lseek(fd,1024,SEEK_SET);
 write(fd,image,512);
 fclose(filen);
 
filen=fopen(argv[4],"r");
 fread(image,1,512,filen);
 lseek(fd,1536,SEEK_SET);
 write(fd,image,512);
 lseek(fd,1536,SEEK_SET);
 write(fd,image,512);
 fclose(filen);
 close(fd);
  }}
