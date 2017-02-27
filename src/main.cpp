/* RPLIDAR Bridge Application */
#include "SocketHelper.h"
#include "RPLidarHelper.h"

#include <json.hpp>

// for convenience
using json = nlohmann::json;

int main(int argc, const char *argv[]) {
    SocketHelper socket;
    RPLidarHelper lidar;

    // Initialisation RPLidar
    lidar.init();

    // Initialisation de la socket et attente de connexion.
    socket.init();
    socket.waitConnection();

    bool stop = false;
    while (!stop) {
        string input = socket.getQuery();
        cout << "From client : " << input << endl;

        json v = json::parse(input.c_str());

        if (v["id"] == 1) {
            socket.sendResponse("1");
        } else if (v["id"] == -1) {
            stop = true;
        } else {
            socket.sendResponse("Salut toi !");
        }
    }

    socket.end();

    return 0;
}
