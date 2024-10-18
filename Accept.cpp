#include "Accept.h"
#include "socket.h"
#include "wclient.h"
#include "channel.h"
#include<iostream>

Accept::Accept(EventLoop* _loop) : loop(_loop) {
	 sock = new Socket();
	Wclient *addr = new Wclient("127.0.0.1", 8888);
	sock->bind(addr);
	sock->listen();
	sock->setnonblocking();

	 servChannel = new Channel(loop, sock->getfd());

	std::function<void()> cb = std::bind(&Accept::acceptConnection, this);

	servChannel->setCallback(cb);
	servChannel->enablereading();
	delete addr;
}

Accept::~Accept() {
	delete sock;
	delete servChannel;
}

void Accept::acceptConnection() {
	Wclient* clnt_addr = new Wclient();
	Socket* clnt_sock = new Socket(sock->accept(clnt_addr));
	std::cout << "new client fd" << clnt_sock->getfd() << "IP:" << inet_ntoa(clnt_addr->addr.sin_addr) << "Port:" << ntohs(clnt_addr->addr.sin_port) << std::endl;
	clnt_sock->setnonblocking();
	newConnectionCallback(clnt_sock);
	delete clnt_addr;
}

void Accept::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
	newConnectionCallback = _cb;
}


