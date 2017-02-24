#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class SocketHelper {

public:
  SocketHelper(int port = 50000, int backlogQueue = 5);

  void init();

  const char * connect();
  void response(const char * response);

  void end();

private:
  int serverSockfd, clientSockfd, port, backlogQueue;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

};

#endif // SOCKETHELPER_H
