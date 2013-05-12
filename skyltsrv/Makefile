

all: SkyltController.o skyltclient.o
     g++ -lxnet -lsocket -lposix4 -o skyltsrv SkyltController.o skyltclient.o

SkyltController.o: SkyltController.cc SkyltController.h bs.h
     g++ -c SkyltController.cc

skyltclient.o: skyltclient.cc SkyltController.h
     g++ -c skyltclient.cc

