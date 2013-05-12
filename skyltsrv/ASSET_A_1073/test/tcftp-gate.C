#include <ftp.h>
#include <string.h>

// sunos does not prototype this in stdio.h
extern "C" char* getpass (char* prompt); 

int main (int ac, char** av)
{
  // this is an ftp client
  // It retrieves the file from the host through the 
  // nsc firewall. Donot use this outside of NSC.
  if (ac < 3) {
    cerr << "USAGE: "
      << av [0]
      << " user@hostname remotefilepath [localfilename]\n";
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

  // get a file
  char* p = strrchr (av [2], '/');
  if (p == 0)
    p = av [2];
  else
    p++;
  f->getfile (av [2], av [3] ? av [3]: p);

  // quit
  f->quit ();
}
    
