#include"wclient.h"
#include<string.h>
#include<unistd.h>
Wclient::Wclient() :
	addr_len(sizeof(addr))
{
	memset(&addr, 0, sizeof(addr));
}

Wclient::Wclient(const char* ip, uint16_t post) :
	addr_len(sizeof(addr))
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(post);
	addr_len = sizeof(addr);
}

Wclient::~Wclient() {}

void Wclient::setInetAddr(sockaddr_in _addr, socklen_t _addr_len) {
	addr = _addr;
	addr_len = _addr_len;
}

sockaddr_in Wclient::getAddr() {
	return addr;
}
socklen_t Wclient::getAddr_len() {
	return addr_len;
}