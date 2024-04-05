#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include "unp.h"


int inet_pton_loose(int family, const char* strptr, void* addrptr)
{
	int ret = 0;
	switch (family)
	{
	case AF_INET:
	{
		if ((ret = inet_pton(family, strptr, addrptr)) == 0)
		{
			struct in_addr  in_val;
			if (inet_aton(strptr, &in_val) != INADDR_NONE)
			{
				printf(" success:%d, %d \n", ret, in_val);
			}

		}
		printf(" failed:%d \n", ret);
	}
	break;
	case AF_INET6:
	{
		if ((ret = inet_pton(family, strptr, addrptr)) == 0)
		{
			struct in_addr  in_val;
			if (inet_aton(strptr, &in_val) != INADDR_NONE)
			{
				printf(" success:%d %d\n", ret, in_val);
			}
		}
		printf(" failed:%d \n", ret);
	}
	break;
	}
	return 0;
}

int main(int argc, char** argv)
{
	struct sockaddr_in* sock = (struct sockaddr_in*)calloc(1, sizeof(struct sockaddr_in));
	inet_pton_loose(AF_INET6, argv[1], &sock->sin_addr);
	return 0;
}