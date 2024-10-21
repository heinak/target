#pragma once
#include<arpa/inet.h>

class Wclient {
public:
	Wclient();
	~Wclient();
	Wclient(const char* ip, uint16_t post);

	void setInetAddr(sockaddr_in _addr);
	sockaddr_in getAddr();
	socklen_t getAddr_len();
private:
	sockaddr_in addr;
	socklen_t addr_len;
};