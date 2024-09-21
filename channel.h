#pragma once
#include<sys/epoll.h>

class Epoll;

class Channel {
public:
	Channel(Epoll* _ep, int _fd);
	~Channel();

	void enablereading();

	int getfd();
	uint32_t getEvents();
	uint32_t getRevent();
	bool getInEpoll();
	void setInEpoll();

	void setRevent(uint32_t);
private:
	Epoll* ep;
	int fd;
	uint32_t events;
	uint32_t revents;
	bool inEpoll;
};