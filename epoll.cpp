#include"epoll.h"
#include"erron.h"
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
void Epoll::addFd(int fd,uint32_t op) {
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.fd = fd;
	ev.events = op;
	errif(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "create epoll_ctl erron");
}

//std::vector<epoll_event> Epoll::poll(int timeout) {
//	std::vector<epoll_event> active;
//	int acfd = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
//	errif(acfd == -1, "create epoll_wait erron");
//	for (int i = 0; i < acfd; ++i) {
//		active.push_back(events[i]);
//	}
//	return active;
//}

std::vector<Channel*> Epoll::poll(int timeout) {
	std::vector<Channel*> active;
	int acfd = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
	errif(acfd == -1, "create epoll_wait erron");
	for (int i = 0; i < acfd; ++i) {
		Channel* ch = (Channel*)events[i].data.ptr;
		ch->setRevent(events[i].events);
		active.push_back(ch);
	}
	return active;
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
		// debug("Epoll: add Channel to epoll tree success, the Channel's fd is: ", fd);
	}
	else {
		errif(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
		// debug("Epoll: modify Channel in epoll tree success, the Channel's fd is: ", fd);
	}
}