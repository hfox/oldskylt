// sockinet.C  -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#include <config.h>
#include <sockinet.h>

EXTERN_C_BEGIN
#include <netdb.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
EXTERN_C_END

void	herror(const char*);

sockinetaddr::sockinetaddr () 
{
  sin_family	  = sockinetbuf::af_inet;
  sin_addr.s_addr = htonl(INADDR_ANY);
  sin_port	  = 0;
}

sockinetaddr::sockinetaddr(unsigned long addr, int port_no)
// addr and port_no are in host byte order
{
  sin_family      = sockinetbuf::af_inet;
  sin_addr.s_addr = htonl(addr);
  sin_port	= htons(port_no);
}

sockinetaddr::sockinetaddr(unsigned long addr, const char* sn, const char* pn)
// addr is in host byte order
{
  sin_family      = sockinetbuf::af_inet;
  sin_addr.s_addr = htonl (addr); // Added by cgay@cs.uoregon.edu May 29, 1993
  setport(sn, pn);
}

sockinetaddr::sockinetaddr (const char* host_name, int port_no)
// port_no is in host byte order
{
  setaddr(host_name);
  sin_port = htons(port_no);
}

sockinetaddr::sockinetaddr(const char* hn, const char* sn, const char* pn)
{
  setaddr(hn);
  setport(sn, pn);
}

sockinetaddr::sockinetaddr (const sockinetaddr& sina)
{
  sin_family      = sockinetbuf::af_inet;
  sin_addr.s_addr = sina.sin_addr.s_addr;
  sin_port	= sina.sin_port;
}   

void sockinetaddr::setport(const char* sn, const char* pn)
{
  servent* sp = getservbyname(sn, pn);
  if (sp == 0) {
    perror(sn);
    error ("sockinetaddr: invalid service name");
    exit(1);
  }
  sin_port = sp->s_port;
}

int sockinetaddr::getport () const
{
  return ntohs (sin_port);
}

void sockinetaddr::setaddr(const char* host_name)
{
  if ( (sin_addr.s_addr = inet_addr(host_name)) == -1) {
    hostent* hp = gethostbyname(host_name);
    if (hp == 0) {
      herror("sockinetaddr::sockinetaddr");
      exit(1);
    }
    memcpy(&sin_addr, hp->h_addr, hp->h_length);
    sin_family = hp->h_addrtype;
  }else
    sin_family = sockinetbuf::af_inet;
}

const char* sockinetaddr::gethostname () const
{
  if (sin_addr.s_addr == htonl(INADDR_ANY)) {
    static char hostname[64];
    if (::gethostname(hostname, 63) == -1) {
      perror("in sockinetaddr::gethostname");
      return "";
    }
    return hostname;		
  }
  
  hostent* hp = gethostbyaddr((const char*) &sin_addr,
			      sizeof(sin_addr),
			      family());
  if (hp == 0) {
    herror("sockinetaddr::gethostname");
    return "";
  }
  if (hp->h_name) return hp->h_name;
  return "";
}

sockinetbuf::sockinetbuf(sockbuf::type ty, int proto)
  : sockbuf(af_inet, ty, proto)
{}

sockinetbuf& sockinetbuf::operator = (const sockbuf& si)
{
  this->sockbuf::operator = (si);
  return *this;
  
}

sockinetbuf& sockinetbuf::operator = (const sockinetbuf& si)
{
  this->sockbuf::operator = (si);
  return *this;
}

sockbuf* sockinetbuf::open(sockbuf::type st, int proto)
{
  *this = sockinetbuf(st, proto);
  return this;
}

sockinetaddr sockinetbuf::localaddr() const
{
  sockinetaddr sin;
  int len = sin.size();
  if (::getsockname(rep->sock, sin.addr (), &len) == -1)
    perror("sockinetbuf::localaddr()");
  return sin;
}

int sockinetbuf::localport() const
{
  sockinetaddr sin = localaddr();
  if (sin.family() != af_inet) return -1;
  return sin.getport();
}

const char* sockinetbuf::localhost() const
{
  sockinetaddr sin = localaddr();
  if (sin.family() != af_inet) return "";
  return sin.gethostname();
}


sockinetaddr sockinetbuf::peeraddr() const
{
  sockinetaddr sin;
  int len = sin.size();
  if (::getpeername(rep->sock, sin.addr (), &len) == -1)
    perror("sockinetbuf::peeraddr()");
  return sin;
}

int sockinetbuf::peerport() const
{
  sockinetaddr sin = peeraddr();
  if (sin.family() != af_inet) return -1;
  return sin.getport();
}

const char* sockinetbuf::peerhost() const
{
  sockinetaddr sin = peeraddr();
  if (sin.family() != af_inet) return "";
  return sin.gethostname();
}

int sockinetbuf::bind_until_success (int portno)
// a. bind to (INADDR_ANY, portno)
// b. if success return 0
// c. if failure and errno is EADDRINUSE, portno++ and go to step a.
// d. return errno.
{
  for (;;) {
    sockinetaddr sa ((unsigned long) INADDR_ANY, portno++);
    int eno = bind (sa);
    if (eno == 0)
      return 0;
    if (eno != EADDRINUSE)
      return eno;
  }
}

int sockinetbuf::bind (sockAddr& sa)
{
  return sockbuf::bind (sa);
}

int sockinetbuf::bind ()
{
  sockinetaddr sa;
  return bind (sa);
}

int sockinetbuf::bind (unsigned long addr, int port_no)
// address and portno are in host byte order
{
  sockinetaddr sa (addr, port_no);
  return bind (sa);
}

int sockinetbuf::bind (const char* host_name, int port_no)
{
  sockinetaddr sa (host_name, port_no);
  return bind (sa);
}

int sockinetbuf::bind (unsigned long addr,
		       const char* service_name,
		       const char* protocol_name)
{
  sockinetaddr sa (addr, service_name, protocol_name);
  return bind (sa);
}

int sockinetbuf::bind (const char* host_name,
		       const char* service_name,
		       const char* protocol_name)
{
  sockinetaddr sa (host_name, service_name, protocol_name);
  return bind (sa);
}

int sockinetbuf::connect (sockAddr& sa)
{
  return sockbuf::connect (sa);
}

int sockinetbuf::connect (unsigned long addr, int port_no)
// address and portno are in host byte order
{
  sockinetaddr sa (addr, port_no);
  return connect (sa);
}

int sockinetbuf::connect (const char* host_name, int port_no)
{
  sockinetaddr sa (host_name, port_no);
  return connect (sa);
}

int sockinetbuf::connect (unsigned long addr,
			  const char* service_name,
			  const char* protocol_name)
{
  sockinetaddr sa (addr, service_name, protocol_name);
  return connect (sa);
}

int sockinetbuf::connect (const char* host_name,
			  const char* service_name,
			  const char* protocol_name)
{
  sockinetaddr sa (host_name, service_name, protocol_name);
  return connect (sa);
}

isockinet::isockinet (sockbuf::type ty, int proto)
  : ios (new sockinetbuf (ty, proto))
{}

isockinet::isockinet (const sockbuf& sb)
  : ios (new sockinetbuf (sb))
{}

isockinet::~isockinet ()
{
  delete ios::rdbuf ();
  init (0);
}

osockinet::osockinet (sockbuf::type ty, int proto)
  : ios (new sockinetbuf (ty, proto))
{}

osockinet::osockinet (const sockbuf& sb)
  : ios (new sockinetbuf (sb))
{}

osockinet::~osockinet ()
{
  delete ios::rdbuf ();
  init (0);
}

iosockinet::iosockinet (sockbuf::type ty, int proto)
  : ios (new sockinetbuf (ty, proto))
{}

iosockinet::iosockinet (const sockbuf& sb)
  : ios (new sockinetbuf (sb))
{}

iosockinet::~iosockinet ()
{
  delete ios::rdbuf ();
  init (0);
}

#if !defined (__linux__)

extern "C" int h_errno;
static	char* errmsg[] = {
  ": No error\n",
  ": Host not found\n",
  ": Try again\n",
  ": No recovery\n",		
  ": No address\n"
  ": Unknown error\n"
};
void herror(const char* em)
{
  if (h_errno > 5) h_errno = 5;
  cerr << em << errmsg[h_errno];
}

#endif // !__linux__
