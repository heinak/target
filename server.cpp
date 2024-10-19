//#include <cstdio>
//#include<sys/types.h>
//#include<sys/socket.h>
//#include<arpa/inet.h>
//#include<string.h>
//#include<netinet/in.h>
//#include<errno.h>
//#include<iostream>
//#include<fcntl.h>
//#include<vector>
//#include<sys/types.h>
//#include<sys/stat.h>
//#include<sys/epoll.h>
//#include<unistd.h>
//#include"erron.h"
//#include"socket.h"
//#include"wclient.h"
//#include"epoll.h"
//#include"channel.h"
//
//#define MAX_EVENTS 1024
//#define READ_BUFFER 1024
//
//void setnonblocking(int fd) {
//	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
//}
//void handevents(int);
//
//int main() {
//	Socket* serv_sock = new Socket();
//	Wclient* serv_addr = new Wclient("127.0.0.1",8888);
//	serv_sock->bind(serv_addr);
//	serv_sock->listen();
//	Epoll* ep = new Epoll();
//	serv_sock->setnonblocking();
//	Channel* servChannel = new Channel(ep, serv_sock->getfd();
//	servChannel->enablereading();
//	//ep->addFd(serv_sock->getfd(), EPOLLIN | EPOLLET);
//	while (true) {
//		std::vector<Channel*> events = ep->poll();
//		int nfs = events.size();
//		for (int i = 0; i < nfs; ++i) {
//			int chfd = events[i]->getfd();
//			if (chfd == serv_sock->getfd()) {
//				Wclient* clnt_addr = new Wclient();
//				Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));
//				std::cout << "new client fd: " << clnt_sock->getfd() << " ip: " << inet_ntoa(clnt_addr->addr.sin_addr) << " post: " << ntohs(clnt_addr->addr.sin_port) << std::endl;
//				clnt_sock->setnonblocking();
//				Channel* clntChannel = new Channel(ep, clnt_sock->getfd());
//				clntChannel->enablereading();
//				//ep->addFd(clnt_sock->getfd(), EPOLLIN | EPOLLET);
//			}
//			else if (events[i]->getRevent() & EPOLLIN) {
//				handevents(events[i]->getfd());
//			}
//			else {
//				std::cout << "something else complete" << std::endl;
//			}
//		}
//	}
//	delete serv_sock;
//	delete serv_addr;
//	return 0;
//}
//
//void handevents(int sockfd) {
//	char buf[READ_BUFFER];
//	while (true) {
//		memset(&buf, 0, sizeof(buf));
//		ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//		if (bytes_read > 0) {
//			std::cout << "get message :" << buf << std::endl;
//		}
//		else if (bytes_read == -1 && errno == EINTR) {
//			std::cout << "continue reading" << std::endl;
//			continue;
//		}
//		else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
//			std::cout << "finish reading once,errno is:" << strerror(errno) << std::endl;
//			break;
//		}
//		else if (bytes_read == 0) {
//			std::cout << "EOF,client fd: " << sockfd << "disconnected" << std::endl;
//			close(sockfd);
//			break;
//		}
//		else {
//			std::cout << "designing" << std::endl;
//		}
//	}
//}


#include "Eventloop.h"
#include "Serverr.h"


int main() {
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);
    loop->loop();
    delete loop;
    delete server;
    return 0;
}