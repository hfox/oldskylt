// tdinread.cc. Test for -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#include <sockinet.h>

int main(int ac, char** av)
{
    isockinet	isin (sockbuf::sock_dgram);
    
    isin->bind ();
    
    cout << "localhost = " << isin->localhost() << endl
	 << "localport = " << isin->localport() << endl;
    
    char		buf[256];
    int		n;
    
    isin >> n;
    cout << av[0] << ": ";
    while(n--) {
	isin >> buf;
	cout << buf << ' ';
    }
    cout << endl;

    return 0;
}


