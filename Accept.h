#pragma once
#include<functional>
class EventLoop;
class Socket;
class Wclient;
class Channel;
class Accept
{
public:
    Accept(EventLoop* _loop);
    ~Accept();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
private:
    EventLoop* loop;
    Socket* sock;
    Wclient* addr;
    Channel* servChannel;
};

