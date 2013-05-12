// pipestream.h -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#ifndef _PIPESTREAM_H
#define	_PIPESTREAM_H

#include <sockstream.h>

class ipipestream: public isockstream {
protected:
    ipipestream (): ios (0) {}
public:
    ipipestream (const char* cmd);
    ~ipipestream () { delete ios::rdbuf (); ios::init (0); }
};

class opipestream: public osockstream {
protected:
    opipestream ():ios(0) {}
public:
    opipestream (const char* cmd);
    ~opipestream () { delete ios::rdbuf (); ios::init (0); }
};

class iopipestream: public iosockstream {
private:
    iopipestream(const iopipestream& sp); // no defintion provided
    iopipestream&	operator = (iopipestream&); // no definition provided
    
protected:
    int		sp[2]; // socket pair
    // childs pid if this is parent and
    // 0 if this is child if object is created through
    // iopipstream (sockbuf::type, int).
    // -1 otherwise;
    pid_t	cpid;
    iopipestream* next;  // next in the chain
    
    static iopipestream* head; // list to take care of by fork()
    
public:
    iopipestream(sockbuf::type ty=sockbuf::sock_stream, int proto=0);
    iopipestream(const char* cmd);
    ~iopipestream () { delete ios::rdbuf (); ios::init (0); }
    
    pid_t pid () const { return cpid; } // returns cpid
    static pid_t fork();     // sets cpid of all iopipestream* in the head
};

#endif	// _PIPESTREAM_H
