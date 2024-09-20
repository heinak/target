#pragma once
#include<arpa/inet.h>

class Wclient {
public:	
	Wclient() ;
	~Wclient() ;
	Wclient(const char* ip, uint16_t post);
	socklen_t len;
	sockaddr_in addr;
};