// protocol.h -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <sockinet.h>

class protocol: public iosockstream {
public:
  enum p_name {
    nil = 0,
    tcp = sockbuf::sock_stream,
    udp = sockbuf::sock_dgram
  };

  class protocolbuf: public sockinetbuf {
  private:
    protocol::p_name pn;

    int bind (sockAddr& sa) { return sockbuf::bind (sa); }
    int connect (sockAddr& sa) { return sockbuf::connect (sa); }
   
  public:
    protocolbuf (sockbuf& si): sockinetbuf (si), pn (protocol::nil) {}
    protocolbuf (protocol::p_name pname)
      : sockinetbuf ((sockbuf::type) pname, 0), pn (pname) {}


    int                 bind () { serve_clients (); return 0; }
    int                 connect ();
    int                 connect (unsigned long addr);
    int                 connect (const char* host);
    int                 connect (const char* host, int portno);

    const char*         protocol_name () const;

    virtual void        serve_clients (int portno = -1) = 0;
    virtual const char* rfc_name () const = 0;
    virtual const char* rfc_doc  () const = 0;
  };

  protocol (): ios (0) {}
};

inline const char* protocol::protocolbuf::protocol_name () const
{
  char* ret = "";
  if (pn == protocol::tcp)
    ret = "tcp";
  if (pn == protocol::udp)
    ret = "udp";
  return ret;
}

inline int protocol::protocolbuf::connect ()
{
  if (pn != protocol::nil)
    return sockinetbuf::connect (localhost (), rfc_name (), protocol_name ());
  return -1;
}

inline int protocol::protocolbuf::connect (unsigned long addr)
     // addr is in host byte order
{
  if (pn != protocol::nil)
    return sockinetbuf::connect (addr, rfc_name (), protocol_name ());
  return -1;
}

inline int protocol::protocolbuf::connect (const char* host)
{
  if (pn != protocol::nil)
    return sockinetbuf::connect (host, rfc_name (), protocol_name ());
  return -1;
}

inline int protocol::protocolbuf::connect (const char* host, int portno)
{
  if (pn != protocol::nil)
    return sockinetbuf::connect (host, portno);
  return -1;
}

#endif // PROTOCOL_H
