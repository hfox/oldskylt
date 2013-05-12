// sockunix.h -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#ifndef _SOCKUNIX_H
#define	_SOCKUNIX_H

#include <sockstream.h>
#include <sys/un.h>

class sockunixaddr: public sockAddr, public sockaddr_un {
public:
    			sockunixaddr (const char* path="/tmp/unix_socket");
    			sockunixaddr (const sockunixaddr& suna);
    			operator void* () const { return (sockaddr_un*)this; }

    int 		size () const { return sizeof (sockaddr_un); }
    int 		family () const { return sun_family; }
    sockaddr*		addr() const {return (sockaddr*)((sockaddr_un*)this);}
};

class sockunixbuf: public sockbuf {
protected:
    sockunixbuf&	operator = (const sockbuf& su);
    sockunixbuf&        operator = (const sockunixbuf& su);
public:
    enum domain { af_unix = AF_UNIX };
    
    			sockunixbuf (const sockbuf& su): sockbuf(su) {}
			sockunixbuf (const sockunixbuf& su): sockbuf (su) {}
    			sockunixbuf (sockbuf::type ty, int proto=0);

    sockbuf*		open (sockbuf::type, int proto=0);

    virtual int		bind (sockAddr& sa);
    int			bind (const char* path);
    virtual int		connect (sockAddr& sa);
    int			connect (const char* path);
};

class isockunix: public isockstream
{
public:
    			isockunix (const sockbuf& sb);
    			isockunix (sockbuf::type ty=sockbuf::sock_stream,
	       			   int proto=0);
                        ~isockunix();

    sockunixbuf*	operator -> () { return (sockunixbuf*)rdbuf (); }
};

class osockunix: public osockstream
{
public:
    			osockunix (const sockbuf& sb);
    			osockunix (sockbuf::type ty=sockbuf::sock_stream,
				   int proto=0);
                        ~osockunix ();

    sockunixbuf*	operator -> () { return (sockunixbuf*)rdbuf (); }
};

class iosockunix: public iosockstream
{
public:
    			iosockunix (const sockbuf& sb);
    			iosockunix (sockbuf::type ty=sockbuf::sock_stream,
				    int proto=0);
			~iosockunix ();

    sockunixbuf*	operator -> () { return (sockunixbuf*)rdbuf (); }
};

#endif	// _SOCKUNIX_H
