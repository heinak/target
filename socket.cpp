#include"socket.h"
#include"erron.h"
#include"wclient.h"
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>


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
	sockaddr_in _addr = addr->getAddr();
	socklen_t addr_len = addr->getAddr_len();
	int bfd = ::bind(fd, (sockaddr*)&_addr, addr_len);
	errif(bfd == -1, "create bind erron");
	addr->setInetAddr(_addr, addr_len);
}

void Socket::listen() {
	int lfd = ::listen(fd, SOMAXCONN);
	errif(lfd == -1, "create listen erron");
}
void Socket::setnonblocking() {
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(Wclient* addr) {
	sockaddr_in _addr;
	socklen_t addr_len = sizeof(_addr);
	memset(&_addr, 0, sizeof(_addr));
	int acfd = ::accept(fd, (sockaddr*)&_addr, &addr_len);
	errif(acfd == -1, "create accept erron");
	addr->setInetAddr(_addr,addr_len);
	return acfd;
}

void Socket::connect(Wclient* addr)
{
	sockaddr_in _addr = addr->getAddr();
	socklen_t addr_len = addr->getAddr_len();
	int connfd = ::connect(fd, (sockaddr*)&_addr, addr_len);
	errif(connfd == -1, "create accept erron");
}

int Socket::getfd() {
	return fd;
}