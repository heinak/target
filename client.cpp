#include<sys/types.h>
#include<sys/socket.h>
#include<cstring>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>
#include<arpa/inet.h>
#include"erron.h"
using namespace std;

#define BUFFER_SIZE 1024

int main() {


	int clintfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clintfd < 0) {
		errif(clintfd == -1, "create socket erron");
	}
	else {
		cout << "creat socket succese" << endl;
	}

	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(8888);

	//int connfd = connect(clintfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
	int connfd = connect(clintfd, (sockaddr*)&sockAddr, sizeof(sockaddr));
	if (connfd < 0) {
		errif(connfd == -1, "create connsct erron");
	}
	else {
		cout << "creat connect succese" << endl;
	}	
	while (true) {
		char buf[BUFFER_SIZE];
		memset(buf, 0, sizeof(buf));
		std::cin >> buf;
		ssize_t write_bybes = write(clintfd, buf, sizeof(buf));
		if (write_bybes == -1) {
			std::cout << "socket already disconnected" << std::endl;
			break;
		}
		memset(&buf, 0, sizeof(buf));
		ssize_t read_bybes = read(clintfd, buf, sizeof(buf));
		if (read_bybes > 0) {
			std::cout << "message from server: " << buf << std::endl;
		}
		else if (read_bybes == 0) {
			std::cout << "server disconnectde"<< std::endl;
			break;
		}
		else if (read_bybes == -1) {
			errif(true, "socket read erron");
			close(clintfd);
		}
	}
	close(clintfd);
	return 0;

}