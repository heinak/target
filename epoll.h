#pragma once
#include<sys/epoll.h>
#include<vector>

class Epoll {
public:
	Epoll() ;
	~Epoll() ;
	void addFd(int fd,uint32_t op);
	std::vector<epoll_event> poll(int timeout = -1);
private:
	int epollfd;
	epoll_event* events;
};