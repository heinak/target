#pragma once
#include<functional>
#include<sys/epoll.h>
class Socket;
class EventLoop;

class Channel {
public:
	Channel(EventLoop* _loop, int _fd);
	~Channel();
	void handleEvent();
	void enablereading();

	int getfd();
	uint32_t getEvents();
    //uint32_t getRevent();
	bool getInEpoll();
	void setInEpoll(bool _in=true);

	//void setRevent(uint32_t);

	void setCallback(std::function<void()>);

	uint32_t getReady();
	uint32_t setReady(uint32_t n);
	void setUseThreadPool(bool use = true);
	void useET();
private:
	EventLoop* loop;
	int fd;
	uint32_t events;
	//uint32_t revents;
	bool inEpoll;
	//std::function<void()> callback;

	uint32_t ready;
	bool useThreadPool;
	std::function<void()> readCallback;
	std::function<void()> writeCallback;
};