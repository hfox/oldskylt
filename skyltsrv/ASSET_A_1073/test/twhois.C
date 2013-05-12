#include <sockinet.h>

int main(int ac, char** av)
{
	if (ac != 2 && ac != 3) {
		cerr << "USAGE: " << av[0] << " [ -l ] user_name\n";
		return 1;
	}
	
	iosockinet	sio (sockbuf::sock_stream);

	if (ac == 3) {
	    if (av[1][0] != '-' || av[1][1] != 'l') {
		cerr << "USAGE: " << av[0] << " [ -l ] user_name\n";
		return 1;
	    }
	    // use local whois server
	    sio->connect ("128.143.2.20", "whois", "tcp"); 
	    sio << av[2] << "\r\n" << flush;
	} else {
	    sio->connect ("nic.ddn.mil", "whois", "tcp");
	    sio << av[1] << "\r\n" << flush;
	}

	char buf[1024];
	while ( sio.getline(buf, 1023) ) cout << buf << endl;
	cout << endl;

	return 0;
}
