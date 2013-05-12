//----------------------------------------------------------------------------
// File: Comm-Dvr.cc
// Author: Bala Swaminathan 5/19/94
// This is a test file for Communication Objects
// 
// Copyright 1994, Washington University in St. Louis
//----------------------------------------------------------------------------

static const char* sscid =
"@(#) Comm-Dvr.cc, testing CommObj and CommController, (C) Wustl 1994";

#define PRO_A_PORT 9999
#define PRO_B_PORT 8888
#define HOSTNAME "larc"

void PRO_A();
void PRO_B();

extern "C" {
  int fork (...);
  int exit (...);
  int sleep (...);
}

#include <string.h>
#include <iostream.h>
#include "commObj.h"

char *strings[] = {"Santa", "Claus", "Is", "Coming", "To", "Town"};

const int cMAXMESGDATA = 2040;
struct Mesg_New {
  Mesg_New (const char* s=NULL, int ty = 0) {
    if (s) init (s, ty); else init ("<default>", ty);
  }
  void init (const char* s, int ty = 0) {
    len = strlen(s);
    qitem.type = ty;
    strcpy (qitem.data, s);
  }
  void print () {
    cout << "len = " << len << endl;
    cout << "struct Q_item" << endl;
    cout << "  type = " << qitem.type << endl
      << "  data = " << (qitem.data[cMAXMESGDATA-1]='\0', qitem.data) << endl;
  }
  int len;
  struct Q_item {
    int type;
    char data[cMAXMESGDATA];
  } qitem;
};

typedef Mesg_New* Mesg_New_Ptr;

#define SERVSBUF(cc) (cc.server())
CommController cc_a (PRO_A_PORT);  // create PRIMARY sockets for both
CommController cc_b (PRO_B_PORT);  //    the protocols we are testing.

main ()
{
  if (fork() == 0) { // child process
    PRO_B ();
    exit (10);
  } else { // parent
    PRO_A ();
    exit (5);
  }
}

void
PRO_A ()
{
  Mesg_New mn;
  cout << "PRO_A: serv.host = " << SERVSBUF(cc_a)->localhost() << endl;
  cout << "PRO_A: serv.port = " << SERVSBUF(cc_a)->localport() << endl;
  // using our controller, now accept the connection
  CommObj co = cc_a.accept();
  cout << "PRO_A: co.local.host = " << (co).localhost() << endl;
  cout << "PRO_A: co.local.port = " << (co).localport() << endl;
  cout << "PRO_A: co.peer.host  = " << (co).peerhost() << endl;
  cout << "PRO_A: co.peer.port  = " << (co).peerport() << endl;
  for (int i=0; i<6; i++) {
    co.read (&mn, sizeof (mn));
    cout << "PRO_A receives:--- " << endl;
    mn.print ();
  }
  mn.init ("PRO_A: THE END", 's');
  co.write (&mn, sizeof (mn));
}

void
PRO_B ()
{
  Mesg_New mn;
  cout << "PRO_B: serv.host = " << SERVSBUF(cc_b)->localhost() << endl;
  cout << "PRO_B: serv.port = " << SERVSBUF(cc_b)->localport() << endl;
  // using our controller, let's connect to PRO_A
  CommObj co;
  sleep (1);
  co.connect (HOSTNAME, PRO_A_PORT);
  cout << "PRO_B: co.local.host = " << (co).localhost() << endl;
  cout << "PRO_B: co.local.port = " << (co).localport() << endl;
  cout << "PRO_B: co.peer.host  = " << (co).peerhost() << endl;
  cout << "PRO_B: co.peer.port  = " << (co).peerport() << endl;
  for (int i=0; i<6; i++) {
    mn.init (strings[i], i);
    co.write (&mn, sizeof (mn));
  }
  co.read (&mn, sizeof (mn));
  mn.print ();
  cout << "PRO_B: THE END" << endl;
}

