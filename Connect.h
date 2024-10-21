#pragma once
#include <functional>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connect
{
public:
    Connect(EventLoop* _loop, Socket* _sock);
    ~Connect();

    void handleReadEvent(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);

    void send(int sockfd);
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;

    std::string* inBuffer;
    Buffer* readBuffer;
};

