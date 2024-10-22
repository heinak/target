#include"socket.h"
#include"erron.h"
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include <errno.h>
//#include <netinet/in.h>
#include <arpa/inet.h>


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
	//socklen_t addr_len = addr->getAddr_len();
	int bfd = ::bind(fd, (sockaddr*)&_addr, sizeof(addr));
	errif(bfd == -1, "create bind erron");
	//addr->setInetAddr(_addr, addr_len);
}

void Socket::listen() {
	int lfd = ::listen(fd, SOMAXCONN);
	errif(lfd == -1, "create listen erron");
}
void Socket::setnonblocking() {
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(Wclient* addr) {
	int clnt_sockfd = -1;
	sockaddr_in _addr;
    socklen_t addr_len = sizeof(_addr);
	memset(&_addr, 0, sizeof(_addr));

	if (fcntl(fd, F_GETFL) & O_NONBLOCK) {
		while (true) {
			clnt_sockfd = ::accept(fd, (sockaddr*)&_addr, &addr_len);
			if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
				continue;
			}
			else if (clnt_sockfd == -1) {
				errif(true, "socket accept error");
			}
			else {
				break;
			}
		}
	}
	else {
		clnt_sockfd = ::accept(fd, (sockaddr*)&_addr, &addr_len);
		errif(clnt_sockfd == -1, "socket accept error");
	}

	//errif(acfd == -1, "create accept erron");
	addr->setInetAddr(_addr);
	return clnt_sockfd;
}

void Socket::connect(Wclient* _addr)
{
	sockaddr_in addr = _addr->getAddr();
	//socklen_t addr_len = addr->getAddr_len();

	if (fcntl(fd, F_GETFL) & O_NONBLOCK) {
		while (true) {
			int ret = ::connect(fd, (sockaddr*)&addr, sizeof(addr));
			if (ret == 0) {
				break;
			}
			else if (ret == -1 && (errno == EINPROGRESS)) {
				continue;
			}
			else if (ret == -1) {
				errif(true, "socket connect error");
			}
		}
	}
	else {
		int connfd = ::connect(fd, (sockaddr*)&_addr, sizeof(addr));
		errif(connfd == -1, "socket connect error");
	}

	//int connfd = ::connect(fd, (sockaddr*)&_addr, sizeof(addr));
	//errif(connfd == -1, "create accept erron");
}

int Socket::getfd() {
	return fd;
}


Wclient::Wclient() {
	bzero(&addr, sizeof(addr));
}
Wclient::Wclient(const char* _ip, uint16_t _port) {
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(_ip);
	addr.sin_port = htons(_port);
}

Wclient::~Wclient() {
}

void Wclient::setInetAddr(sockaddr_in _addr) {
	addr = _addr;
}

sockaddr_in Wclient::getAddr() {
	return addr;
}

char* Wclient::getIp() {
	return inet_ntoa(addr.sin_addr);
}

uint16_t Wclient::getPort() {
	return ntohs(addr.sin_port);
}