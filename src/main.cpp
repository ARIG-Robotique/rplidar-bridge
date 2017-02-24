/* RPLIDAR Bridge Application */

#include "sockethelper.h"

int main(int argc, const char * argv[]) {
  SocketHelper socket;
  socket.init();

  socket.connect();
  socket.response("Salut toi !");

  socket.end();

  return 0;
}
