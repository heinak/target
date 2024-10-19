#pragma once
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>


class Wclient;
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