#include"epoll.h"
#include"erron.h"
#include"channel.h"
#include<unistd.h>
#include<string.h>

#define MAX_EVENTS 1000

Epoll::Epoll() :epollfd(-1), events(nullptr) {
	epollfd = epoll_create1(0);
	errif(epollfd == -1, "create epollfd error");
	events = new epoll_event[MAX_EVENTS];
	memset(events, 0, sizeof(events)*MAX_EVENTS);
}

Epoll::~Epoll(){
	if (epollfd == -1) {
		close(epollfd);
		epollfd = -1;
	}
	delete[] events;
}

void Epoll::deleteChannel(Channel* channel)
{
	int fd = channel->getfd();
	int eppfd = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
	errif(eppfd == -1, "delete epoll erron");
	channel->setInEpoll(false);
}

//void Epoll::addFd(int fd,uint32_t op) {
//	struct epoll_event ev;
//	bzero(&ev, sizeof(ev));
//	ev.data.fd = fd;
//	ev.events = op;
//	errif(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "create epoll_ctl erron");
//}
//
//
//std::vector<Channel*> Epoll::poll(int timeout) {
//	std::vector<Channel*> active;
//	int acfd = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
//	errif(acfd == -1, "create epoll_wait erron");
//	for (int i = 0; i < acfd; ++i) {
//		Channel* ch = (Channel*)events[i].data.ptr;
//		ch->setRevent(events[i].events);
//		active.push_back(ch);
//	}
//	return active;
//}

std::vector<Channel*> Epoll::poll(int timeout) {
	std::vector<Channel*> activeChannels;
	int nfds = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
	errif(nfds == -1, "epoll wait error");
	for (int i = 0; i < nfds; ++i) {
		Channel* ch = (Channel*)events[i].data.ptr;
		ch->setReady(events[i].events);
		activeChannels.push_back(ch);
	}
	return activeChannels;
}

void Epoll::updateChannel(Channel* channel) {
	int fd = channel->getfd();
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = channel;
	ev.events = channel->getEvents();
	if (!channel->getInEpoll()) {
		errif(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
		channel->setInEpoll();
	}
	else {
		errif(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
	}
}