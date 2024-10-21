#include <iostream>
#include <unistd.h>
#include <string>
#include "erron.h"
#include "Buffer.h"
#include "wclient.h"
#include "socket.h"

using namespace std;

#define BUFFER_SIZE 1024

int main() {
    Socket* sock = new Socket();
    Wclient* addr = new Wclient("127.0.0.1", 1234);
    sock->connect(addr);

    int sockfd = sock->getfd();

    Buffer* sendBuffer = new Buffer();
    Buffer* readBuffer = new Buffer();

    while (true) {
        sendBuffer->getline();
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            cout << "socket already disconnected, can't write any more!" << endl;
            break;
        }
        int already_read = 0;
        char buf[1024];    
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0) {       
                cout << "server disconnected!" << endl;
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->size()) {
                cout << "message from server : " << readBuffer->c_str() << endl;
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    delete sock;
    return 0;
}