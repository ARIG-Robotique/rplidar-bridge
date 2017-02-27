#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include <unistd.h>
#include <string>
#include <netinet/in.h>

using namespace std;

class SocketHelper {

public:
    SocketHelper(int port = 50000, int backlogQueue = 5);

    void init();

    void waitConnection();

    string getQuery(void);

    void sendResponse(string response);

    void end();

private:
    int serverSockfd, clientSockfd, port, backlogQueue;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

};

#endif // SOCKETHELPER_H
