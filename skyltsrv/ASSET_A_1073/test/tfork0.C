#include <iostream.h>
#include <Fork.h>

static void print (char* name, pid_t child)
{
  if (child) {
    cerr << "Parent " << getppid () << "; "
      << name << ' ' << getpid () << "; Child " << child << ";\n";
  } else if (0) {
    cerr << "Parent " << getppid () << "; "
      << name << "'s child " << getpid () << ";\n";
  }
}

int main (int ac, char** av)
{
  Fork A (1, 1);
  
  print ("A", A.process_id ());
  if (A.is_child ()) {
    sleep (120);
    return 0x8;
  }
  sleep (30);
  return 0;
}
    
