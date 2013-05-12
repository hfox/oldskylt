#include <echo.h>
#include <stdlib.h>

int main (int ac, char** av)
{
  if (ac < 3) {
    cerr << "USAGE: " << av [0] << " remote-host portno string...\n";
    return 1;
  }

  int portno = atoi (av [2]);

  echo e(protocol::tcp);

  if (portno == 0)
    e->connect (av [1]);
  else
    e->connect (av [1], portno);

  cout << e->rfc_name () << ' ' << e->rfc_doc () << endl;

  for (int i = 0; i < ac; i++) {
    char buf [256];

    cout << "sending: " << av [i] << endl;

    e << av [i] << endl;

    e.getline (buf, 255);

    cout << "got back: " << buf << endl;
  }

  return 0;
}
