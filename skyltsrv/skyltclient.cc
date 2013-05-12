#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "bs.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
//#include "SkyltController.h"
#define path "skyltdata"

typedef unsigned char byte;

bool getFile() {
  servent* sp;
  sockaddr_in isin,from;
  int s;
  int sd = 0;

  //SkyltController* skylt = SkyltController::GetController();

  if ((sp=getservbyport(7613,NULL)) == NULL) {
    perror("Cannot get server entry");
    return false;
  }

  isin.sin_family = AF_INET;
  isin.sin_port = sp->s_port;
  isin.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Cannot open socket");
    return false;
  }

  if (bind(s, (sockaddr*)&isin, sizeof(isin)) < 0) {
    perror("Bind");
    return false;
  }

  if (listen(s,10) < 0) {
    perror ("listen");
    return false;
  }

  int len = sizeof(isin);

  while (true) {
    printf("Listening for clients...\n");

    int clisock;
    if ((clisock = accept(s, (sockaddr*)&from, &len)) < 0) {
      perror("Accept");
      return false;
    }

    printf("Client accepted. Copying data...\n");

    //skylt->Kill();
    if (sd != 0)
      close(sd);

    if ((sd = open(path,O_WRONLY | O_CREAT)) == -1) {
      perror("File");
    }

    byte buffer[4096];
    int bytesread;
    int test = 0;
    do {
      bytesread = read(clisock,buffer,sizeof(buffer));
      printf("%d bytes read\n",bytesread);
      write(sd, buffer, bytesread);
    } while (bytesread != 0);

    close(sd);

    close(clisock);


    if ((sd = open(path,O_RDONLY)) == -1) {
      perror("File");
    }

    printf("\nData copied\n");

    //skylt->Start(sd);
  }
}

int main(void) {

  getFile();

  return 0;
}
