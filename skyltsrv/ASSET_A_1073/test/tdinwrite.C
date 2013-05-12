// tdinwrite.cc. Test for -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#include <sockinet.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char** av)
{
	if (ac < 3) {
		cerr << "USAGE: " << av[0] << " thostname port-number "
		     << "data ... " << endl;
		return 1;
	}

	osockinet osin (sockbuf::sock_dgram);

	osin->connect (av[1], atoi (av[2]));

	cout << "local: " << osin->localport() << ' ' << osin->localhost() << endl
	     << "peer:  " << osin->peerport() << ' ' << osin->peerhost() << endl;

	// test socket options
	cout << "socket type = " << osin->gettype() << endl;
	cout << "socket linger time = " << osin->linger(10) << endl;
	cout << "socket linger time = " << osin->linger(0)  << endl;
	cout << "socket linger time = " << osin->linger()   << endl;
// 	cout << "socket send bufsz = " << osin->sendbufsz() << endl;
// 	cout << "socket recv bufsz = " << osin->recvbufsz() << endl;
	cout << "socket keepalive  = " << osin->keepalive(1) << endl;
	cout << "socket keepalive  = " << osin->keepalive(0) << endl;
	cout << "socket clearerror = " << osin->clearerror() << endl;
	cout << "socket debug = " << osin->debug(1) << endl;
	cout << "socket debug = " << osin->debug(0) << endl;
        cout << "socket reuse = " << osin->reuseaddr(1) << endl;
	cout << "socket reuse = " << osin->reuseaddr(0) << endl;

#ifdef __linux__
	int timeo = 0;
	osin->getopt(sockbuf::so_sndtimeo, &timeo, sizeof(timeo));
	cout << "socket sendtimeo = " << timeo << endl;
	osin->getopt(sockbuf::so_rcvtimeo, &timeo, sizeof(timeo));
	cout << "socket recvtimeo = " << timeo << endl;
#endif

	osin << ac-3; av += 3;
	while(*av) osin << *av++ << ' ';
	osin << endl;
	
	return 0;
}

	
	
