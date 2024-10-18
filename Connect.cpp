#include "socket.h"
#include "channel.h"
#include "Connect.h"
#include <functional>
#include <unistd.h>
#include <iostream>


#define READ_BUFFER 1024

Connect::Connect(EventLoop* _loop,Socket*_sock) : loop(_loop), sock(_sock),channel(nullptr) {
    channel = new Channel(loop, sock->getfd());
    std::function<void()> cb = std::bind(&Connect::handleReadEvent, this, sock->getfd());
    channel->setCallback(cb);
    channel->enablereading();
}

Connect::~Connect()
{
    delete channel;
    delete sock;
}

void Connect::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(&buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            std::cout << "get message :" << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
            break;
        }
        else if (bytes_read == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "finish reading once,errno is:" << strerror(errno) << std::endl;
            break;
        }
        else if (bytes_read == 0) {
            std::cout << "EOF,client fd: " << sockfd << "disconnected" << std::endl;
            deleteConnectionCallback(sock);
            break;
        }
        else {
            std::cout << "designing" << std::endl;
        }
    }
}

void Connect::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}