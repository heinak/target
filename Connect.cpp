#include "socket.h"
#include "channel.h"
#include "Connect.h"
#include "Buffer.h"
#include <functional>
#include <unistd.h>
#include <iostream>
#include <string>

#define READ_BUFFER 1024

Connect::Connect(EventLoop* _loop,Socket*_sock) : loop(_loop), sock(_sock),channel(nullptr),inBuffer(new std::string()),readBuffer(nullptr) {
    channel = new Channel(loop, sock->getfd());

    channel->enablereading();
    channel->useET();

    std::function<void()> cb = std::bind(&Connect::handleReadEvent, this, sock->getfd());
    channel->setCallback(cb);
    channel->setUseThreadPool(true);

    readBuffer = new Buffer();
}

Connect::~Connect()
{
    delete channel;
    delete sock;
    //delete inBuffer;
    delete readBuffer;
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

void Connect::send(int sockfd) {
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int  data_size = readBuffer->size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= bytes_write;
    }
}