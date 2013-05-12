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
#include <signal.h>

#include "SkyltController.h"

SkyltController SkyltController::m_inst;

SkyltController* SkyltController::GetController() {
  return &m_inst;
}

void SkyltController::Start(int inp) {
  m_pid = fork();
  if (isChild()) {
    m_inp = inp;
    printf("Child running\n");
    signal(SIGHUP,HUPHandler);
    usleep(10000000);
  }
}

void SkyltController::Restart() {
  if (!m_inst.isChild()) {
    kill(m_pid,SIGHUP);
    Start(m_inp);
  }
}

void SkyltController::Kill() {
  if (!m_inst.isChild()) {
    kill(m_inst.m_pid,SIGHUP);
    printf("Parent: process killed\n");
  }
}  

SkyltController::SkyltController() {
  if (isChild()) {
    m_nBS = 0;
    m_pSkylt = 0;
    m_pid = 0;
    printf("Child: Constructor called\n");
  }
  else
    printf("Parent: Constructor called\n");
}

SkyltController::~SkyltController() {
  if (isChild()) {
    printf("Child: destructor called. Freeing resources.\n");
    if (m_pSkylt) {
      if(munmap((void*)m_pSkylt, 2048) == -1) {
	perror("munmap");
      }
    }
    if (m_nBS)
      close(m_nBS);
  }
  else {
    printf("Parent: destructor called. Shutting down.\n");
    Kill();
  }
}

void SkyltController::HUPHandler(int) {
  printf("Child: HUP received. Exiting.\n");
  exit(1);
}

void SkyltController::Run() {

  if ((m_nBS = open("/dev/bs0", O_RDWR)) == -1) {
    perror("Cannot open basskylt device.");
    return;
  }

  if((m_pSkylt = (byte *) mmap(NULL, 2048, (PROT_READ|PROT_WRITE), 
                            MAP_SHARED, m_nBS, 0)) == (void*)-1) {
    perror("Cannot map skylt device memory");
    return;
  }

  byte bank = 1;
  ioctl(m_nBS, BSSETBANK, bank);
  ioctl(m_nBS, BSSETMODE, MODE_1BIT);

  unsigned char cmd;
  bool eof = false;
  while (!eof) {
    if (read(m_inp, &cmd, 1) != 1) {
      eof = true;
      break;
    }

    switch (cmd) {
    case SETMODE: {
      byte m;
      if (read(m_inp, &m, 1) == 1)
	ioctl(m_nBS, BSSETMODE, &m);
      else
	eof = true;
      break;
    }
    case SLEEP: {
      int t;
      if (read(m_inp, &t, 4) == 4)
	usleep(t*1000);
      else
	eof = true;
      break;
    }
    case SETFRAME: {
      byte buffer[288];
      if (read(m_inp, buffer, 288) == 288) {
	bank = (bank+1) & 3;
	memcpy(m_pSkylt+bank*512,buffer,288);
	ioctl(m_nBS, BSSETBANK, bank);
      }
      else
	eof = true;
      break;
    }
    }
    if (eof) {
      lseek(m_inp, 0, SEEK_SET);
      eof = false;
    }
  }
}

