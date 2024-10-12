#include"epoll.h"
#include"channel.h"
#include"Eventloop.h"

Channel::Channel(EventLoop*_loop, int _fd)
	: loop(_loop),
	  fd(_fd),
	  events(0),
	revents(0),
	inEpoll(false)
{}

Channel::~Channel(){}

void Channel::handleEvent() {
	callback();
}

void Channel::enablereading(){
	events = EPOLLIN | EPOLLET;
	loop->updateChannel(this);
}

int Channel::getfd() {
	return fd;
}

uint32_t Channel::getEvents() {
	return events;
}
uint32_t Channel::getRevent() {
	return revents;
}

bool Channel::getInEpoll() {
	return inEpoll;
}

void Channel::setInEpoll() {
	inEpoll = true;
}

// void Channel::setEvent(uint32_t _ev){
//     events = _ev;
// }

void Channel::setRevent(uint32_t _ev) {
	revents = _ev;
}
void Channel::setCallback(std::function<void()> _cb) {
	callback = _cb;
}