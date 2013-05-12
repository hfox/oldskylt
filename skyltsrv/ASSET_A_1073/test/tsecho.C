#include <echo.h>
#include <stdlib.h>

int main (int ac, char** av)
{
  if (ac != 2) {
    cerr << "USAGE: " << av [0] << " portno\n";
    return 1;
  }

  echo server (protocol::tcp);
  server->serve_clients (atoi (av [1]));
  return 2;
}
