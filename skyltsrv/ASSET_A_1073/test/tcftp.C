#include <ftp.h>

// sunos does not prototype this in stdio.h
extern "C" char* getpass (char* prompt); 

int main (int ac, char** av)
{
  // this is an ftp client
  // It retrieves the file from the host
  if (ac != 4) {
    cerr << "USAGE: " << av [0] << " hostname user filename\n";
    return 1;
  }

  ftp f (&cout);

  // establish connection
  f->connect (av [1]);
  f->get_response (); // get the connection response

  // set access 
  f->user (av [2]);
  f->passwd (getpass ("passwd: "));

  // get help
  f->help ();

  // set representation type to image
  f->rep_type (ftp::rt_image);

  // list the home directory
  f->list ();

  // quit
  f->quit ();
}
    
