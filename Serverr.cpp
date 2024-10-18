#include "Serverr.h"
#include "socket.h"
#include "wclient.h"
#include "channel.h"
#include "Accept.h"
#include <functional>
#include <unistd.h>
#include <iostream>


#define READ_BUFFER 1024

Server::Server(EventLoop* _loop) : loop(_loop),accept(nullptr){
    accept = new Accept(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    accept->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete accept;
}

void Server::handleReadEvent(int sockfd) {
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
        			close(sockfd);
        			break;
        		}
        		else {
        			std::cout << "designing" << std::endl;
        		}
        	
    }
}

void Server::newConnection(Socket* serv_sock) {
    Wclient* clnt_addr = new Wclient();      //会发生内存泄露！没有delete
    Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));       //会发生内存泄露！没有delete
    std::cout << "new client fd" << clnt_sock->getfd() << "IP:" << inet_ntoa(clnt_addr->addr.sin_addr) << "Port:" << ntohs(clnt_addr->addr.sin_port) << std::endl;
    //printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getfd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel* clntChannel = new Channel(loop, clnt_sock->getfd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getfd());
    clntChannel->setCallback(cb);
    clntChannel->enablereading();
}