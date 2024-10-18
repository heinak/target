#include "Accept.h"
#include "socket.h"
#include "wclient.h"
#include "channel.h"

Accept::Accept(EventLoop* _loop) : loop(_loop) {
	 sock = new Socket();
	addr = new Wclient("127.0.0.1", 8888);
	sock->bind(addr);
	sock->listen();
	sock->setnonblocking();

	 servChannel = new Channel(loop, sock->getfd());

	std::function<void()> cb = std::bind(&Accept::acceptConnection, this);

	servChannel->setCallback(cb);
	servChannel->enablereading();

}

Accept::~Accept() {
	delete sock;
	delete addr;
	delete servChannel;
}

void Accept::acceptConnection() {
	newConnectionCallback(sock);
}

void Accept::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
	newConnectionCallback = _cb;
}


