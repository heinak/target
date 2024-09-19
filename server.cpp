#include <cstdio>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/epoll.h>
#include<unistd.h>
#include"erron.h"
using namespace std;
#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
	
	int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketfd < 0) {
		errif(socketfd == -1, "create socket erron");
	}
	else {
		cout << "create socket succese" << endl;
	}
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(8080);
	int Bind = bind(socketfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));

	if (Bind < 0) {
		errif(Bind == -1, "create bind erron");
	}
	else {
		cout << "creat bind susses" << endl;
	}


	int Listen = listen(socketfd, 5);


	if (Listen < 0) {
		errif(Listen == -1, "create listen erron");
	}
	else {
		cout << "socket listening......" << endl;
	}
	
	int epfd = epoll_create1(0);
	if (epfd < 0) {
		errif(epfd == -1, "create epoll_createl erron");
	}
	else {
		cout << "socket listening......" << endl;
	}
	struct epoll_event events[MAX_EVENTS], ev;
	bzero(&events, sizeof(events));
	bzero(&ev, sizeof(ev));

	ev.data.fd = socketfd;
	ev.events = EPOLLIN | EPOLLET;

	setnonblocking(socketfd);

	epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &ev);

	while (true) {
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (nfds < 0) {
			errif(nfds == -1, "create epoll_wait erron");
		}
		else {
			cout << "socket listening......" << endl;
		}

		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == socketfd) {
				struct sockaddr_in clnt_addr;
				bzero(&clnt_addr, sizeof(clnt_addr));
				socklen_t clnt_addr_len = sizeof(clnt_addr);

				int clnt_sockfd = accept(socketfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
				if (clnt_sockfd< 0) {
					errif(clnt_sockfd == -1, "create accept erron");
				}
				else {
					cout << "new client fd:" << clnt_sockfd << "ip:" << inet_ntoa(clnt_addr.sin_addr) << "port" << ntohs(clnt_addr.sin_port) << endl;
				}
				 
				bzero(&ev, sizeof(ev));

				ev.data.fd = clnt_sockfd;
				ev.events = EPOLLIN | EPOLLET;

				setnonblocking(clnt_sockfd);

				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
			}
			else if (events[i].events & EPOLLIN) {
				char buf[READ_BUFFER];
				while (true) {
					bzero(&buf, sizeof(buf));
					ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
					if (bytes_read > 0) {
						cout << "get message :" << buf << endl;
					}
					else if (bytes_read == -1 && errno == EINTR) {
						cout << "continue reading" << endl;
						continue;
					}
					else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
						cout << "finish reading once,errno is:" << errno << endl;
						break;
					}
					else if (bytes_read == 0) {
						cout << "EOF,client " << events[i].data.fd << "disconnected" << endl;
						break;
					}
				}
			}
			else {
				cout << "designing" << endl;
			}
		}

	}


	
	/*while (true) {
		int connfd = accept(socketfd, nullptr, nullptr);
		if (connfd < 0) {
			errif(connfd == -1, "create accept erron");
		}
		
		char buf[1024] = { 0 };
		size_t len = recv(connfd, buf, sizeof(buf), 0);
		cout << "套接字信息:" << connfd << "接收的信息:" << buf << endl;
		send(connfd, buf, len, 0);
	}*/

	close(socketfd);

	return 0;
}