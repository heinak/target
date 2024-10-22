#pragma once
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>


class Wclient {
public:
	Wclient();
	~Wclient();
	Wclient(const char* ip, uint16_t post);


	void setInetAddr(sockaddr_in _addr);
	sockaddr_in getAddr();
	char* getIp();
	uint16_t getPort();

private:
	sockaddr_in addr;
	//socklen_t addr_len;
};

class Socket {
public:
	Socket();
	Socket(int);
	~Socket() ;

	void bind(Wclient *addr);
	void listen();
	int accept(Wclient* addr);
	void connect(Wclient*);

	void setnonblocking();
	int getfd();
private:
	int fd;
};