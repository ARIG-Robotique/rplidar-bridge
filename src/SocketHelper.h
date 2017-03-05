#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include "Constantes.h"
#include <netinet/in.h>

class SocketHelper {

public:
    SocketHelper(int port = 50000, int backlogQueue = 5);

    void init();

    void waitConnection();

    JsonQuery getQuery(void);
    void sendResponse(JsonResult response);

    void end();

private:
    static const int DEFAULT_SOCKET_FD = -1;

    int serverSockfd, clientSockfd, port, backlogQueue;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

};

#endif // SOCKETHELPER_H
