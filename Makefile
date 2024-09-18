server:
	g++	erron.cpp	server.cpp -o server && g++	erron.cpp	client.cpp -o client
clean:
	rm	server	&&	rm	client