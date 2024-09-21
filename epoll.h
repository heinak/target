#pragma once
#include<sys/epoll.h>
#include<vector>
#include"channel.h"
class Epoll {
public:
	Epoll() ;
	~Epoll() ;
	void addFd(int fd,uint32_t op);
	void updateChannel(Channel*);
	//std::vector<epoll_event> poll(int timeout = -1);
	std::vector<Channel*> poll(int timeout = -1);
private:
	int epollfd;
	epoll_event* events;
};