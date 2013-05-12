// pipestream.C -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10


#include <config.h>

#include <pipestream.h>
#include <unistd.h>
#include <sys/socket.h>

// environ is not given a declaration in sun's <unistd.h>
extern char** environ;

// child closes s2 and uses s1
// parent closes s1 and uses s2

enum domain { af_unix = 1 };

iopipestream* iopipestream::head = 0;

static sockbuf* createpipestream (const char* cmd, int mode)
{
  int sockets[2];
  if ( ::socketpair (af_unix, sockbuf::sock_stream, 0, sockets) == -1 ) {
    sock_error ("pipestream", "socketpair");
    return 0;
  }
  
  pid_t pid = vfork ();
  if (pid == -1) {
    sock_error ("pipestream", "fork");
    return 0;
  }
  
  if (pid == 0) {
    // child process
    if (::close (sockets[1]) == -1)
      sock_error ("pipestream", "child close 1");
    if ((mode & ios::in) && ::dup2 (sockets[0], 1) == -1) {
      sock_error ("pipestream", "child dup2 1");
      _exit (0x70);
    }
    if ((mode & ios::out) && ::dup2 (sockets[0], 0) == -1) {
      sock_error ("pipestream", "child dup2 0");
      _exit (0x71);
    }
    if (::close (sockets[0]) == -1)
      sock_error ("pipestream", "child close 0");
    const char*	argv[4];
    argv[0] = "/bin/sh";
    argv[1] = "-c";
    argv[2] = cmd;
    argv[3] = 0;
    execve ("/bin/sh", (char**) argv, environ);
    sock_error ("pipestream", "execve");
    _exit (0x7f);
  }
  // parent process
  sockbuf* s = new sockbuf (sockets[1]);
  if (::close (sockets[0]) == -1)
    sock_error ("pipestream", "parent close 0");
  if ( !(mode & ios::out) ) s->shutdown (sockbuf::shut_write);
  if ( !(mode & ios::in) ) s->shutdown (sockbuf::shut_read);
  return s;
}

ipipestream::ipipestream (const char* cmd)
  : ios (0)
{
  init (createpipestream (cmd, ios::in));
}

opipestream::opipestream (const char* cmd)
  : ios (0)
{
  init (createpipestream (cmd, ios::out));
}

iopipestream::iopipestream (const char* cmd)
  : ios (0), cpid(-1), next (0)
{
  init (createpipestream (cmd, ios::in|ios::out));
}

iopipestream::iopipestream(sockbuf::type ty, int proto)
  : ios (0), cpid(-1), next (head)
{
  if ( ::socketpair(af_unix, ty, proto, sp) == -1 ) {
    error ("iopipestream-socketpair ");
    return;
  }
  
  head = this;	
}

pid_t	iopipestream::fork()
{
  pid_t pid = :: vfork ();
  if ( pid == -1 ) {
    sock_error ("iopipestream", "fork");
    return pid;
  } else if (pid > 0) {
    // parent process
    while (head) {
      head->cpid = pid;
      head->init (new sockbuf (head->sp[0]));
      if (::close (head->sp[1]) == -1)
	sock_error ("iopipestream", "parent close 0");
      head = head->next;
    }
    
  } else {
    // child process
    while (head) {
      head->cpid = 0;
      head->init (new sockbuf (head->sp[1]));
      if (::close (head->sp[0]) == -1)
	sock_error ("iopipestream", "child close 0");
      head = head->next;
    }
  }
  return pid;
}	

