#include "Serverr.h"
#include "socket.h"
#include "wclient.h"
#include "channel.h"
#include "Accept.h"
#include "Connect.h"
#include <functional>
#include <unistd.h>
#include <iostream>

//#define read_buffer 1024

Server::Server(EventLoop* _loop) : loop(_loop), accept(nullptr) {
    accept = new Accept(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    accept->setNewConnectionCallback(cb);
}


Server::~Server()
{
    delete accept;
}

//void Server::handleReadEvent(int sockfd) {
//    char buf[READ_BUFFER];
//    while (true) {    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
//        memset(&buf, 0, sizeof(buf));
//        		ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//        		if (bytes_read > 0) {
//        			std::cout << "get message :" << buf << std::endl;
//                    write(sockfd, buf, sizeof(buf));
//                    break;
//        		}
//        		else if (bytes_read == -1 && errno == EINTR) {
//        			std::cout << "continue reading" << std::endl;
//        			continue;
//        		}
//        		else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
//        			std::cout << "finish reading once,errno is:" << strerror(errno) << std::endl;
//        			break;
//        		}
//        		else if (bytes_read == 0) {
//        			std::cout << "EOF,client fd: " << sockfd << "disconnected" << std::endl;
//        			close(sockfd);
//        			break;
//        		}
//        		else {
//        			std::cout << "designing" << std::endl;
//        		}
//    }
//}




void Server::newConnection(Socket* serv_sock) {
    if (serv_sock->getfd() != -1) {
        Connect* conn = new Connect(loop, serv_sock);
        std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connect[serv_sock->getfd()] = conn;
    }
}

void Server::deleteConnection(Socket* sock)
{
    if (sock->getfd() != -1) {
        Connect* conn = connect[sock->getfd()];
        connect.erase(sock->getfd());
        delete conn;
    }
}
