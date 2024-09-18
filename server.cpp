#include <cstdio>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<netinet/in.h>
#include<errno.h>
#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
using namespace std;


int main() {

	int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketfd < 0) {
		cout << "creat socket error " << strerror(errno) << endl;
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
		cout << "creat bind error " << strerror(errno) << endl;

	}
	else {
		cout << "creat bind susses" << endl;
	}


	int Listen = listen(socketfd, 5);


	if (Listen < 0) {

		cout << "creat listen error " << strerror(errno) << endl;
		return 1;
	}
	else {
		cout << "socket listening......" << endl;
	}
	while (true) {
		int connfd = accept(socketfd, nullptr, nullptr);
		if (connfd < 0) {
			cout << "creat accept error " << strerror(errno) << endl;
			return 1;
		}
		char buf[1024] = { 0 };
		size_t len = recv(connfd, buf, sizeof(buf), 0);
		cout << "套接字信息:" << connfd << "接收的信息:" << buf << endl;
		send(connfd, buf, len, 0);
	}

	close(socketfd);

	return 0;
}