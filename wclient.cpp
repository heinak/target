#include"wclient.h"
#include<string.h>
#include<unistd.h>
Wclient::Wclient() :
	len(sizeof(addr))
{
	memset(&addr, 0, sizeof(addr));
}
Wclient::Wclient(const char* ip, uint16_t post) :
	len(sizeof(addr))
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(post);
	len = sizeof(addr);
}

Wclient::~Wclient() {}
