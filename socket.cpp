#include"socket.h"
#include"erron.h"
#include"wclient.h"
#include<fcntl.h>
#include<sys/socket.h>
#include<string.h>

Socket::Socket() :fd(-1) {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	errif(fd == -1, "create socker erron");
}
Socket::Socket(int _fd) :fd(_fd) {
	errif(fd == -1, "create socker erron");
}
Socket::~Socket() {
	if (fd == -1) {
		close(fd);
		fd = -1;
	}
	
}
void Socket::bind(Wclient *addr) {
	int bfd = ::bind(fd, (sockaddr*)&addr->addr, addr->len);
	errif(bfd == -1, "create bind erron");
}

void Socket::listen() {
	int lfd = ::listen(fd, SOMAXCONN);
	errif(lfd == -1, "create listen erron");
}
void Socket::setnonblocking() {
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(Wclient* addr) {
	int acfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->len);
	errif(acfd == -1, "create accept erron");
	return acfd;
}

int Socket::getfd() {
	return fd;
}