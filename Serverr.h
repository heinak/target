#pragma once
#include <map>

class EventLoop;
class Socket;
class Accept;
class Connect;
class Server
{
private:
    EventLoop* loop;
    Accept* accept;
    std::map<int, Connect*> connect;
public:
    Server(EventLoop* _loop);
    ~Server();

    //void handleReadEvent(int);
    void newConnection(Socket* serv_sock);

    void deleteConnection(Socket* sock);
};