// sockinet.h -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#ifndef _SOCKINET_H
#define	_SOCKINET_H

#include <sockstream.h>
#include <netinet/in.h>

class sockinetaddr: public sockAddr, public sockaddr_in {
protected:
    void		setport (const char* sn, const char* pn="tcp");
    void		setaddr (const char* hn);
public:
	                ~sockinetaddr () {}
    			sockinetaddr ();
    			sockinetaddr (unsigned long addr, int port_no=0);
    			sockinetaddr (const char* host_name, int port_no=0);
    			sockinetaddr (unsigned long addr,
				      const char* service_name,
				      const char* protocol_name="tcp");
    			sockinetaddr (const char* host_name,
		 		      const char* service_name,
				      const char* protocol_name="tcp");
                        sockinetaddr (const sockinetaddr& sina);    
    			operator void* () const { return (sockaddr_in*)this; }

    int 		size() const { return sizeof (sockaddr_in); }
    int 		family() const { return sin_family; }
    sockaddr*		addr() const {return (sockaddr*)((sockaddr_in*)this); }
    
    int			getport() const;
    const char* 	gethostname() const;
};

class sockinetbuf: public sockbuf {
protected:
    sockinetbuf&	operator=(const sockbuf& si); // needs to be fixed
    sockinetbuf&        operator=(const sockinetbuf& si); // needs fixing
public:
    enum domain { af_inet = AF_INET };
    
    			sockinetbuf (const sockbuf& si): sockbuf(si) {}
			sockinetbuf (const sockinetbuf& si): sockbuf (si) {}
    			sockinetbuf (sockbuf::type ty, int proto=0);
    
    sockbuf*		open (sockbuf::type, int proto=0);
    
    sockinetaddr	localaddr() const;
    int			localport() const;
    const char*		localhost() const;
    
    sockinetaddr	peeraddr() const;
    int			peerport() const;
    const char*		peerhost() const;

    int                 bind_until_success (int portno);

    virtual int		bind (sockAddr& sa);
    int			bind ();
    int			bind (unsigned long addr, int port_no=0);
    int			bind (const char* host_name, int port_no=0);
    int			bind (unsigned long addr,
			      const char* service_name,
			      const char* protocol_name="tcp");
    int			bind (const char* host_name,
			      const char* service_name,
			      const char* protocol_name="tcp");

    virtual int		connect (sockAddr& sa);
    int			connect (unsigned long addr, int port_no=0);
    int			connect (const char* host_name, int port_no=0);
    int			connect (unsigned long addr,
				 const char* service_name,
				 const char* protocol_name="tcp");
    int			connect (const char* host_name,
				 const char* service_name,
				 const char* protocol_name="tcp");

};

class isockinet: public isockstream
{
public:
    			isockinet (const sockbuf& sb);
    			isockinet (sockbuf::type ty=sockbuf::sock_stream,
	       			   int proto=0);
                        ~isockinet ();

    sockinetbuf*	rdbuf () { return (sockinetbuf*)ios::rdbuf (); }
    sockinetbuf*	operator -> () { return rdbuf (); }
};

class osockinet: public osockstream
{
public:
    			osockinet (const sockbuf& sb);
    			osockinet (sockbuf::type ty=sockbuf::sock_stream,
				   int proto=0);
                        ~osockinet ();

    sockinetbuf*	rdbuf () { return (sockinetbuf*)ios::rdbuf (); }
    sockinetbuf*	operator -> () { return rdbuf (); }
};

class iosockinet: public iosockstream
{
public:
    			iosockinet (const sockbuf& sb);
    			iosockinet (sockbuf::type ty=sockbuf::sock_stream,
				    int proto=0);
                        ~iosockinet ();

    sockinetbuf*	rdbuf () { return (sockinetbuf*)ios::rdbuf (); }
    sockinetbuf*	operator -> () { return rdbuf (); }
};

#endif	// _SOCKINET_H
