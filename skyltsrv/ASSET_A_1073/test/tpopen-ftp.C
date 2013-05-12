#include <pipestream.h>

int main()
{
        char buf[128];
        iopipestream p("ftp");
        p->unbuffered();
        p << "help\r\n" << flush;
        while ( p.getline(buf, 127) ) cout << buf << endl;
        return 0;
}
