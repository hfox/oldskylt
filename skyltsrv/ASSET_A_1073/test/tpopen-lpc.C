#include <pipestream.h>

int main()
{
        char buf[128];
        iopipestream p("lpc");
        p << "help\nquit\n" << flush;
        while ( p.getline(buf, 127) ) cout << buf << endl;
        return 0;
}
