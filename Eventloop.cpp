#include "Eventloop.h"
#include "epoll.h"
#include "channel.h"
#include "Theadpool.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false),threadpool(nullptr) {
    ep = new Epoll();
    threadpool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep;
    delete threadpool;
}


void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}

void EventLoop::addThread(std::function<void()> fun)
{
    threadpool->add(fun);
}
