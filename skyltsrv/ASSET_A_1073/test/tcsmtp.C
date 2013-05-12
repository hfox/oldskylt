#include <smtp.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

int main (int ac, char** av)
{
  if (ac < 4) {
    cerr << "USAGE: " << av [0] << " host recipient files...\n";
    return 1;
  }

  smtp client (&cout);

  // establish connection
  client->connect (av [1]);
  client->helo ();

  // get help
  client->help ();

  // get the FROM address
  char    sender [256];
  passwd* pw = getpwuid( getuid() );
  sprintf(sender, "%s@%s", pw->pw_name, client->localhost());

  // send the files
  for (int i = 3; i < ac; i++) {
    client->mail (sender);
    client->rcpt (av [2]);
    client->data (av [i]);
    client->rset ();
  }

  // finally quit
  client->quit ();

  return 0;
}
