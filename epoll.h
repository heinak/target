#pragma once
#include<sys/epoll.h>
#include<vector>
#include"channel.h"
class Channel;
class Epoll {
public:
	Epoll() ;
	~Epoll() ;
	void deleteChannel(Channel*);
	void updateChannel(Channel*);

	std::vector<Channel*> poll(int timeout = -1);

private:
	int epollfd;
	epoll_event* events;
};