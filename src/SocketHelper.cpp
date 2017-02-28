#include "SocketHelper.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <memory.h>

using namespace std;

SocketHelper::SocketHelper(int port, int backlogQueue) {
    this->port = port;
    this->backlogQueue = backlogQueue;
}

void SocketHelper::init() {
    cout << "Initialisation de la socket sur le port " << port << endl;

    // create a socket
    // socket(int domain, int type, int protocol)
    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);

    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* setup the host_addr structure for use in bind call */
    // server byte order
    serv_addr.sin_family = AF_INET;

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(port);

    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    // bind() passes file descriptor, the address structure,
    // and the length of the address structure
    // This bind() call will bind the socket to the current IP address on port, portno
    if (bind(serverSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "ERROR on binding" << endl;
        exit(1);
    }

    // This listen() call tells the socket to listen to the incoming connections.
    // The listen() function places all incoming connection into a backlog queue
    // until accept() call accepts the connection.
    listen(serverSockfd, backlogQueue);
}

void SocketHelper::waitConnection() {
    // The accept() call actually accepts an incoming connection
    clilen = sizeof(cli_addr);

    // This accept() function will write the connecting client's address info
    // into the the address structure and the size of that structure is clilen.
    // The accept() returns a new socket file descriptor for the accepted connection.
    // So, the original socket file descriptor can continue to be used
    // for accepting new connections while the new socker file descriptor is used for
    // communicating with the connected client.
    clientSockfd = accept(serverSockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (clientSockfd < 0) {
        cerr << "ERROR on accept" << endl;
        exit(2);
    }
    cout << "Server: got connection from " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << endl;
}

JsonQuery SocketHelper::getQuery(void) {
    char buffer[256];
    bzero(buffer, 256);
    if (read(clientSockfd, buffer, 255) < 0) {
        cerr << "ERROR Reading from socket" << endl;
    }

    JsonQuery q;
    // Controle que le premier caractère n'est pas null, \r ou \n
    if (buffer[0] != 0 && buffer[0] != 10 && buffer[0] != 13) {
        try {
            json jsonValue = json::parse(buffer);
            cout << "Query from client : " << jsonValue.dump(2) << endl;
            q.action = jsonValue["action"];
            q.datas = jsonValue["datas"];
        } catch (const exception & e) {
            cerr << "Erreur de lecture du JSON : " << e.what() << endl;
            q.action = "UNPARSABLE";
        }
    } else {
        cerr << "Empty query from client" << endl;
        q.action = "DATA_INVALID";
    }

    return q;
}

void SocketHelper::sendResponse(JsonResult response) {
    json r;
    r["status"] = response.status;
    r["action"] = response.action;
    r["errorMessage"] = response.errorMessage;
    r["datas"] = response.datas;

    cout << "Response to client : " << r.dump(2) << endl;

    string str = r.dump();
    send(clientSockfd, str.c_str(), str.length(), 0);
}

void SocketHelper::end() {
    close(clientSockfd);
    close(serverSockfd);
}
