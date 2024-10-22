#include <iostream>
#include <unistd.h>
#include <string.h>
#include <functional>
#include "erron.h"
#include "Buffer.h"
#include "socket.h"
#include "Theadpool.h"


void oneClient(int msgs, int wait) {
    Socket* sock = new Socket();
    Wclient* addr = new Wclient("127.0.0.1", 1234);
    sock->connect(addr);

    int sockfd = sock->getFd();

    Buffer* sendBuffer = new Buffer();
    Buffer* readBuffer = new Buffer();

    sleep(wait);
    int count = 0;
    while (count < msgs) {
        sendBuffer->setBuf("I'm client!");
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            std::cout << "socket already disconnected, can't write any more!" << std::endl;
            break;
        }
        int already_read = 0;
        char buf[1024];    //这个buf大小无所谓
        while (true) {
            memest(buf, 0, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0) {         //EOF
                std::cout << "server disconnected!" << std::endl;
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->size()) {
                std::cout << "count:" << count++ << " message from server : " << readBuffer->c_str()) << std::endl;
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    delete sock;
}

int main(int argc, char* argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char* optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
        case 't':
            threads = stoi(optarg);
            break;
        case 'm':
            msgs = stoi(optarg);
            break;
        case 'w':
            wait = stoi(optarg);
            break;
        case '?':
            std::cout << "error optopt: " << optopt << std::endl;
            std::cout << "error opterr: " << opterr << std::endl;
            break;
        }
    }

    ThreadPool* poll = new ThreadPool(threads);
    std::function<void()> func = std::bind(oneClient, msgs, wait);
    for (int i = 0; i < threads; ++i) {
        poll->add(func);
    }
    delete poll;
    return 0;
}
