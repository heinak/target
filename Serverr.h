#pragma once

class EventLoop;
class Socket;
class Accept;
class Server
{
private:
    EventLoop* loop;
    Accept* accept;
public:
    Server(EventLoop*);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket* serv_sock);
};