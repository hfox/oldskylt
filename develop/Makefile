CC= gcc
CFLAGS= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -fno-strength-reduce
DEFS= -D__KERNEL__ -DMODULE -DMODVERSIONS
INCS= -I/usr/src/linux/include -include /usr/src/linux/include/linux/modversions.h

module: bs.o

tests: wmem rmem

bs.o: bs.c
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -c -o bs.o bs.c

rmem: rmem.o
	$(CC) $(CFLAGS) -o rmem rmem.o

wmem: wmem.o myatoi.o
	$(CC) $(CFLAGS) -o wmem wmem.o myatoi.o

