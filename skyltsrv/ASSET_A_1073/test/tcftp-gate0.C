#include <ftp.h>
#include <string.h>

// sunos does not prototype this in stdio.h
extern "C" char* getpass (char* prompt); 

int main (int ac, char** av)
{
  // This is an ftp client.
  // It puts a file at the remote site outside the
  // nsc firewall. Donot use this outside of NSC.
  if (ac != 4) {
    cerr << "USAGE: "
      << av [0]
      << " user@hostname localfilepath remotefilepath\n";
    return 1;
  }

  ftp f (&cout);

  // establish connection
  f->connect ("gatekeeper.nsc.com");
  f->get_response (); // get the connection response

  // set access 
  f->user (av [1]);
  f->passwd (getpass ("Password: "));

  // get help
  f->help ();

  // set representation type to image
  f->rep_type (ftp::rt_image);

  // put a file
  f->putfile (av [2], av [3]);

  // quit
  f->quit ();
}
    
