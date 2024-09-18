#include<sys/types.h>
#include<sys/socket.h>
#include<cstring>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>
#include<arpa/inet.h>
#include"erron.h"
using namespace std;


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
	sockAddr.sin_port = htons(8080);

	//int connfd = connect(clintfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
	int connfd = connect(clintfd, (sockaddr*)&sockAddr, sizeof(sockaddr));
	if (connfd < 0) {
		errif(connfd == -1, "create connsct erron");
	}
	else {
		cout << "creat connect succese" << endl;
	}	
	
    const char* str = "hello world";
	send(connfd, str, strlen(str), NULL);
	char meg[1024] = { 0 };
	recv(connfd, meg, 1024, 0);
	cout << "接到的信息为：" << meg << endl;

	close(clintfd);
	return 0;

}