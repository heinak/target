#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Connect
{
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connect(EventLoop* _loop, Socket* _sock);
    ~Connect();

    void handleReadEvent(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};

