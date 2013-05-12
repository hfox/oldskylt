// sockunix.C -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#include <sockunix.h>
#include <sys/socket.h>
#include <string.h>

sockunixaddr::sockunixaddr (const char* path)
{
  sun_family = sockunixbuf::af_unix;
  ::strcpy (sun_path, path);
}

sockunixaddr::sockunixaddr (const sockunixaddr& suna)
{
  sun_family = sockunixbuf::af_unix;
  ::strcpy (sun_path, suna.sun_path);
}

sockunixbuf::sockunixbuf (sockbuf::type ty, int proto)
  : sockbuf (af_unix, ty, proto)
{}

sockunixbuf& sockunixbuf::operator = (const sockbuf& su)
{
  this->sockbuf::operator = (su);
  return *this;
}

sockunixbuf& sockunixbuf::operator = (const sockunixbuf& su)
{
  this->sockbuf::operator = (su);
  return *this;
}

sockbuf* sockunixbuf::open (type st, int proto)
{
  *this = sockunixbuf (st, proto);
  return this;	
}

int sockunixbuf::bind (sockAddr& sa)
{
  return sockbuf::bind (sa);
}

int sockunixbuf::bind (const char* path)
{
  sockunixaddr sa (path);
  return bind (sa);
}

int sockunixbuf::connect (sockAddr& sa)
{
  return sockbuf::connect (sa);
}

int sockunixbuf::connect (const char* path)
{
  sockunixaddr sa (path);
  return connect (sa);
}

isockunix::isockunix (sockbuf::type ty, int proto)
  : ios (new sockunixbuf (ty, proto))
{}

isockunix::isockunix (const sockbuf& sb)
  : ios (new sockunixbuf (sb))
{}

isockunix::~isockunix ()
{
  delete ios::rdbuf ();
  ios::init (0);
}

osockunix::osockunix (sockbuf::type ty, int proto)
  : ios (new sockunixbuf (ty, proto))
{}

osockunix::osockunix (const sockbuf& sb)
  : ios (new sockunixbuf (sb))
{}

osockunix::~osockunix ()
{
  delete ios::rdbuf ();
  ios::init (0);
}

iosockunix::iosockunix (sockbuf::type ty, int proto)
  : ios (new sockunixbuf (ty, proto))
{}

iosockunix::iosockunix (const sockbuf& sb)
  : ios (new sockunixbuf (sb))
{}

iosockunix::~iosockunix ()
{
  delete ios::rdbuf ();
  ios::init (0);
}
