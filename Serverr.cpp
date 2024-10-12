#include "Serverr.h"
#include "socket.h"
#include "wclient.h"
#include "channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define READ_BUFFER 1024

Server::Server(EventLoop* _loop) : loop(_loop) {
Socket* serv_sock = new Socket();
Wclient* serv_addr = new Wclient("127.0.0.1", 8888);
serv_sock->bind(serv_addr);
serv_sock->listen();
serv_sock->setnonblocking();

Channel* servChannel = new Channel(loop, serv_sock->getfd());
std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
servChannel->setCallback(cb);
servChannel->enablereading();

}

Server::~Server()
{

}

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {    //����ʹ�÷�����IO����ȡ�ͻ���buffer��һ�ζ�ȡbuf��С���ݣ�ֱ��ȫ����ȡ���
        memset(&buf, 0, sizeof(buf));
        		ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        		if (bytes_read > 0) {
        			std::cout << "get message :" << buf << std::endl;
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
}

void Server::newConnection(Socket* serv_sock) {
    Wclient* clnt_addr = new Wclient();      //�ᷢ���ڴ�й¶��û��delete
    Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));       //�ᷢ���ڴ�й¶��û��delete
    std::cout << "new client fd" << clnt_sock->getfd() << "IP:" << inet_ntoa(clnt_addr->addr.sin_addr) << "Port:" << ntohs(clnt_addr->addr.sin_port�� << std::endl;
    //printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getfd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel* clntChannel = new Channel(loop, clnt_sock->getfd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getfd());
    clntChannel->setCallback(cb);
    clntChannel->enablereading();
}