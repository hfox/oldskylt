#include <sockinet.h>
#include <stdlib.h>

static int send_cmd(const char*, iosockstream&);
static int get_text(istream&);

int main()
{
	iosockinet     sio(sockbuf::sock_stream);
	sio->connect("murdoch.acc.virginia.edu", "nntp", "tcp");

	send_cmd(0, sio);
	
	send_cmd("HELP", sio); get_text(sio);
	send_cmd("QUIT", sio);

	return 0;
}

int send_cmd(const char* cmd, iosockstream& s)
{
	if (cmd) s << cmd << "\r\n" << flush;

	char buf[256];
	s.getline(buf, 255);

	cout << buf << endl;

	if (buf[0] == '4' || buf[1] == '5') return 1;
	return 0;
}

int get_text(istream& s)
{
	char buf[256];
	while (s.getline(buf, 255))
		if (buf[0] == '.') {
			if (buf[1] == '.') cout << buf+1 << endl;
			else return 0;
		}else
			cout << buf << endl;
	return 1;
}
		
