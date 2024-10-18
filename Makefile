server:
	g++ erron.cpp	server.cpp	epoll.cpp	wclient.cpp	socket.cpp	channel.cpp	Eventloop.cpp	Serverr.cpp	Accept.cpp	-o	server	&&	\
	g++	erron.cpp	client.cpp	-o	client	