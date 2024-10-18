#pragma once
#include<functional>
#include<sys/epoll.h>

class EventLoop;

class Channel {
public:
	Channel(EventLoop* _loop, int _fd);
	~Channel();
	void handleEvent();
	void enablereading();

	int getfd();
	uint32_t getEvents();
	uint32_t getRevent();
	bool getInEpoll();
	void setInEpoll();

	void setRevent(uint32_t);

	void setCallback(std::function<void()>);
private:
	EventLoop* loop;
	int fd;
	uint32_t events;
	uint32_t revents;
	bool inEpoll;
	std::function<void()> callback;
};