#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include <iostream>

using namespace std;
int inet_pton_loose(int family, const char* strptr, void* addrptr)
{
	int ret = 0;
	switch (family)
	{
	case AF_INET:
	{
		if (ret = inet_pton(family, strptr, addrptr) == 0)
		{
			struct in_addr  in_val;
			if (inet_aton(strptr, &in_val) != INADDR_NONE)
			{
				cout << "success." << inet_addr(strptr);
			}

		}
		cout << "failed :" << ret;
	}
	break;
	case AF_INET6:
	{
		if (ret = inet_pton(family, strptr, addrptr) == 0)
		{
			struct in_addr  in_val;
			if (inet_aton(strptr, &in_val) != INADDR_NONE)
			{
				cout << "success." << inet_addr(strptr);
			}
		}
		cout << "failed :" << ret;
	}
	break;
	}
	return 0;
}

int main(int argc, char** argv)
{
	sockaddr_in* sock = new sockaddr_in();
	inet_pton_loose(AF_INET, "127.0.0.1", &sock->sin_addr);
	return 0;
}