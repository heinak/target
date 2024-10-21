server:
	g++ erron.cpp	server.cpp	epoll.cpp	wclient.cpp	socket.cpp	channel.cpp	Eventloop.cpp	Serverr.cpp	Accept.cpp	Connect.cpp	Buffer.cpp	Theadpool.cpp	-o	server	&&	\
	g++	erron.cpp	client.cpp	socket.cpp	Buffer.cpp	wclient.cpp	-o	client
th:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ erron.cpp Buffer.cpp socket.cpp wclient.cpp Theadpool.cpp \
	-pthread \
	test.cpp -o test