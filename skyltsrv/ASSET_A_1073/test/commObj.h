//----------------------------------------------------------------------------
// File: CommObj.hh
// Author: Bala Swaminathan 5/12/94
// This file contains interface of the Network Communication Object class.
// 
// Copyright 1994, Washington University in St. Louis
//----------------------------------------------------------------------------

// The following CommObj is modeled using the sockets.
// Each PRO creates a CommObj of type CommObj::PRIMARY and this becomes
// its main socket. Later whenever it needs a "new" socket it either
// (1) creates a socket of type CommObj::SECONDARY and then "connects"
// to another PRO's primary socket, or (2) the PRO can "accept" a connection
// from another PRO's secondary socket. This "accepting" a connection, if
// succesful, results in the creation of a socket of type CommObj::SECONDARY.

#ifndef CommObj_hh
#define CommObj_hh

#include "sockinet.h"

class CommObj : public sockinetbuf {
public:
  enum type { PRIMARY, SECONDARY };
public:
  CommObj (const type ty = SECONDARY,    // "do-nothing" constructor
           const int portID=0)          // can specify a port (for PRIMARY)
    : sockinetbuf(sockbuf::sock_stream)  // TCP Sockets
      {
        if (ty == PRIMARY) {
          sockinetaddr addr (INADDR_ANY, portID);
          // this creates addr of INADDR_ANY and port=0
          // i.e. sockets bound to any address can "connect" to us
          bind (addr);    // bind to the above address
          listen ();      // now listen (default 5) for new connections
        }
      }
  CommObj (const sockbuf& sb): sockinetbuf (sb) {}

  const sockinetbuf  sbuf () { return *this; } // for DEUBUGGING purposes
};

// The CommController manages all the CommObj instances in the PROTOCOL.
//    (* the list of CommObj* maintained by CommController are not shown *)

class CommController {
public:
  CommController (int portID=0)           // creates the PERIMARY CommObj
    {
      c_server = new CommObj (CommObj::PRIMARY, portID);
    }
  ~CommController ()                      // close all sockets and delete list
    { delete c_server; }
  CommObj* server ()                       // return "main" socket
    { return c_server; }
  CommObj  accept ()                // accept connection on the SERVER Commobj
    {
      //  return c_server->accept();
      sockinetaddr sa;
      sockinetbuf si = c_server->accept(sa);
      cout << "   accept(): peer.host = " << sa.gethostname() << endl;
      cout << "   accept(): peer.port = " << sa.getport() << endl;
      cout << "     acc: local.host = " << (si).localhost() << endl;
      cout << "     acc: local.port = " << (si).localport() << endl;
      cout << "     acc: peer.host  = " << (si).peerhost() << endl;
      cout << "     acc: peer.port  = " << (si).peerport() << endl;
      return si;
    }
private:
  CommObj*       c_server;          // the server or the main Object
};

#endif CommObj_hh

